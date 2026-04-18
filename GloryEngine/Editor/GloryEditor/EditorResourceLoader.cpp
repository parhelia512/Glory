#include "EditorResourceLoader.h"
#include "AssetCompiler.h"
#include "EditorAssetDatabase.h"
#include "Importer.h"

#include <Debug.h>
#include <JobManager.h>
#include <JobWorkerPool.h>
#include <Resources.h>
#include <AssetArchive.h>
#include <BinaryStream.h>

#include <GloryAssert.h>

namespace Glory::Editor
{
	static Jobs::JobWorkerPool<bool, UUID, std::filesystem::path, Resource*>* ResourceLoaderJobPool = nullptr;

	EditorResourceLoader::EditorResourceLoader(Jobs::JobManager* pJobManager, Debug* pDebug):
		m_pJobManager(pJobManager), m_pDebug(pDebug), m_LoadedResources(nullptr), m_CachedResources(0ull)
	{
		ResourceLoaderJobPool = m_pJobManager->Run<bool, UUID, std::filesystem::path, Resource*>();
	}

	EditorResourceLoader::~EditorResourceLoader()
	{
		ResourceLoaderJobPool = nullptr;
	}

	void EditorResourceLoader::CheckResourceCache()
	{
		/* Go over all cached resources and check if they are outdated */
		const std::filesystem::path cachePath = AssetCompiler::GenerateCompiledResourcePath(0ull).parent_path();
		/* @todo */
		throw std::exception("Not yet implemented!");
	}

	void EditorResourceLoader::QueueLoad(UUID id)
	{
		/* Does a valid cache for the resource exist? */
		std::filesystem::path cachePath;
		std::filesystem::path assetPath;

		const bool hasValidCache = ResourceHasValidCache(id, cachePath, assetPath);
		if (hasValidCache)
		{
			/* Queue a cache load job instead */
			m_pDebug->LogInfo(std::format("EditorResourceLoader: Found valid cache for resource {}", uint64_t(id)));

			/* Load the cache */
			ResourceLoaderJobPool->QueueSingleJob([this](UUID id, std::filesystem::path cachePath, Resource*)
				{ return LoadCacheJob(id, cachePath); }, id, cachePath, nullptr);

			return;
		}

		/* If we are already compiling the root asset we don't need to queue it again */
		if (m_AlreadyCompilingPaths.contains(assetPath)) return;
		m_AlreadyCompilingPaths.emplace(assetPath, 0);

		/* Compile the asset */
		ResourceLoaderJobPool->QueueSingleJob([this](UUID, std::filesystem::path assetPath, Resource*)
			{ return CompileJob(assetPath); }, id, assetPath, nullptr);
	}

	void EditorResourceLoader::QueueUnload(UUID id)
	{
		m_ToUnload.insert(id);
	}

	void EditorResourceLoader::OnUpdate()
	{
		UpdateImportedResources();
		UpdateLoadedResources();
		UpdateCachedResources();
		HandleUnloading();
	}

	bool EditorResourceLoader::ResourceHasValidCache(UUID id, std::filesystem::path& cachePath, std::filesystem::path& assetPath) const
	{
		cachePath = AssetCompiler::GenerateCompiledResourcePath(id);

		AssetLocation location;
		GLORY_ASSERT(EditorAssetDatabase::GetAssetLocation(id, location), "Unknown resource ID");
		assetPath = EditorAssetDatabase::GetAbsoluteAssetPath(location.Path);

		if (!std::filesystem::exists(cachePath)) return false;

		const auto time = std::filesystem::last_write_time(cachePath);
		const uint64_t cacheWriteTime = std::chrono::duration_cast<std::chrono::seconds>(
		time.time_since_epoch()).count();

		const auto assetTime = std::filesystem::last_write_time(assetPath);
		const uint64_t assetWriteTime = std::chrono::duration_cast<std::chrono::seconds>(
			time.time_since_epoch()).count();

		return cacheWriteTime >= assetWriteTime;
	}

	bool EditorResourceLoader::CompileJob(const std::filesystem::path& assetPath)
	{
		/* Import the resource */
		ImportedResource resource = Importer::Import(assetPath);
		if (!resource)
		{
			/* There was an error during importing */
			m_pDebug->LogError(std::format("EditorResourceLoader: Failed to import asset at path {}", assetPath.string()));
			return false;
		}

		const uint32_t index = m_ImportWriteIndex.fetch_add(1) % ImportedResourcesRingBufferSize;
		m_ImportedResources[index] = std::move(resource);
		m_CurrentImportedResourceCount.fetch_add(1);

		return true;
	}

	bool EditorResourceLoader::CacheJob(UUID id, const std::filesystem::path& cachePath, Resource* pResource)
	{
		{
			Utils::BinaryFileStream cacheFile{ cachePath };
			AssetArchive archive{ &cacheFile, AssetArchiveFlags::WriteNew };
			archive.Serialize(pResource);
		}

		const uint32_t index = m_NextCachedResourceWriteIndex.fetch_add(1);
		m_CachedResources[index] = id;
		m_CurrentCachedResourceCount.fetch_add(1);

		return true;
	}

	bool EditorResourceLoader::LoadCacheJob(UUID id, const std::filesystem::path& cachePath)
	{
		{
			Utils::BinaryFileStream cacheFile{ cachePath, true, false };
			AssetArchive archive{ &cacheFile, AssetArchiveFlags::Read };
			archive.Deserialize(m_pDebug, m_pResources->GetResourceTypes());

			for (size_t i = 0; i < archive.Size(); ++i)
			{
				Resource* pNewResource = archive.Get(m_pDebug, i);
				const uint32_t nextResourceIndex = m_LoadedResourceWriteIndex.fetch_add(1);
				m_LoadedResources[nextResourceIndex] = pNewResource;
				m_CurrentLoadedResourceCount.fetch_add(1);
			}
		}
		return true;
	}

	static void ImportIfNew(ImportedResource& resource)
	{
		if (resource.IsNew())
		{
			EditorAssetDatabase::ImportAsset(resource.Path().string(), resource, resource.SubPath(), 0, false);
			return;
		}

		for (size_t i = 0; i < resource.ChildCount(); ++i)
			ImportIfNew(resource.Child(i));
	}

	void EditorResourceLoader::UpdateImportedResources()
	{
		const uint32_t size = m_CurrentImportedResourceCount.load();
		if (size == 0) return;
		/* Process 1 per frame to keep slowdown to an absolute minimum */
		const uint32_t actualSize = m_CurrentImportedResourceCount.fetch_add(-1);
		ImportedResource& resource = m_ImportedResources[m_CurrentImportedResourcesReadIndex];
		++m_CurrentImportedResourcesReadIndex;
		m_CurrentImportedResourcesReadIndex = m_CurrentImportedResourcesReadIndex % ImportedResourcesRingBufferSize;
		if (!resource) return;
		ImportIfNew(resource);
		ProcessImportedResource(resource);
		resource = nullptr;
	}

	void EditorResourceLoader::ProcessImportedResource(ImportedResource& resource)
	{
		for (size_t i = 0; i < resource.ChildCount(); ++i)
		{
			ProcessImportedResource(resource.Child(i));
		}

		Resource* pNewResource = *resource;
		const UUID resourceID = pNewResource->GetUUID();
		if (!m_pResources->AddResource(&pNewResource)) return;

		const std::filesystem::path cachePath = AssetCompiler::GenerateCompiledResourcePath(resourceID);
		m_CachingItems.insert(resourceID);
		/* Dispatch a cache job */
		ResourceLoaderJobPool->QueueSingleJob([this](UUID id, std::filesystem::path cachePath, Resource* pResource)
			{ return CacheJob(id, cachePath, pResource); }, resourceID, cachePath, pNewResource);

		m_pDebug->LogInfo(std::format("EditorResourceLoader: Finished compiling resource {}", uint64_t(resourceID)));
	}

	void EditorResourceLoader::UpdateLoadedResources()
	{
		const uint32_t size = m_CurrentLoadedResourceCount.load();
		if (size == 0) return;

		/* We can process all of them since all we need to do is move them which is fast */
		m_CurrentLoadedResourceCount.fetch_sub(size);
		for (size_t i = 0; i < size; ++i)
		{
			Resource*& pResource = m_LoadedResources[m_CurrentLoadedResourceReadIndex];
			++m_CurrentLoadedResourceReadIndex;
			m_CurrentLoadedResourceReadIndex = m_CurrentLoadedResourceReadIndex % LoadedResourcesRingBufferSize;
			if (!pResource) return;
			if (!m_pResources->AddResource(&pResource))
				delete pResource;
			pResource = nullptr;
		}
	}

	void EditorResourceLoader::UpdateCachedResources()
	{
		const uint32_t size = m_CurrentCachedResourceCount.load();
		if (size == 0) return;

		m_CurrentCachedResourceCount.fetch_sub(size);
		for (size_t i = 0; i < size; ++i)
		{
			UUID& resourceID = m_CachedResources[m_CurrentCachedResourceReadIndex];
			++m_CurrentCachedResourceReadIndex;
			m_CurrentCachedResourceReadIndex = m_CurrentCachedResourceReadIndex % CachedResourcesRingBufferSize;
			if (!resourceID) return;
			m_CachingItems.erase(resourceID);
			resourceID = 0;
		}
	}

	void EditorResourceLoader::HandleUnloading()
	{
		if (m_ToUnload.empty()) return;

		static constexpr size_t maxUnloadsInSingleFrame = 1000;
		std::array<UUID, maxUnloadsInSingleFrame> toUnload;
		size_t currentUnloadCount = 0;

		for (auto iter = m_ToUnload.begin(); iter != m_ToUnload.end(); ++iter)
		{
			if (currentUnloadCount >= maxUnloadsInSingleFrame) break;
			if (m_CachingItems.contains(*iter)) continue;
			toUnload[currentUnloadCount] = *iter;
			++currentUnloadCount;
		}

		for (size_t i = 0; i < currentUnloadCount; ++i)
		{
			m_pResources->UnloadResource(toUnload[i]);
			m_ToUnload.erase(toUnload[i]);
			m_pDebug->LogInfo(std::format("EditorResourceLoader: Resource {} was unloaded", uint64_t(toUnload[i])));
		}
	}
}
