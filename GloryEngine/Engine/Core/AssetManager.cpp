#include "AssetManager.h"
#include "AssetDatabase.h"
#include "GloryContext.h"
#include "Engine.h"
#include "BinaryStream.h"
#include "AssetArchive.h"

#include <Hash.h>

namespace Glory
{
	void AssetManager::GetAsset(UUID uuid, std::function<void(Resource*)> callback)
	{
		Resource* pResource = FindResource(uuid);
		if (pResource)
		{
			callback(pResource);
			return;
		}

		if (ASSET_MANAGER->m_AssetLoadedCallbacks.Contains(uuid))
		{
			if (callback != NULL)
			{
				ASSET_MANAGER->m_AssetLoadedCallbacks.Do(uuid, [&](std::vector<std::function<void(Resource*)>>* callbacks)
				{
					callbacks->push_back(callback);
				});
			}
			return;
		}
		else if (callback != NULL) ASSET_MANAGER->m_AssetLoadedCallbacks.Set(uuid, { callback });

		ASSET_MANAGER->m_pResourceLoadingPool->QueueSingleJob(AssetManager::LoadResourceJob, uuid);
	}

	Resource* AssetManager::GetOrLoadAsset(UUID uuid)
	{
		if (ASSET_MANAGER->m_pLoadingAssets.Contains(uuid)) return nullptr;

		Resource* pResource = FindResource(uuid);
		if (pResource) return pResource;
		if (ASSET_MANAGER->m_AssetLoadedCallbacks.Contains(uuid)) return nullptr;
		ASSET_MANAGER->m_AssetLoadedCallbacks.Set(uuid, { [](Resource*) {} });
		ASSET_MANAGER->m_pResourceLoadingPool->QueueSingleJob(AssetManager::LoadResourceJob, uuid);
		return nullptr;
	}

	Resource* AssetManager::GetAssetImmediate(UUID uuid)
	{
		while (true)
		{
			LoadingLock lock{ uuid };
			if (lock.IsValid) break;
		}

		Resource* pResource = FindResource(uuid);
		if (pResource) return pResource;
		return LoadAsset(uuid);
	}

	void AssetManager::ReloadAsset(UUID uuid)
	{
		UnloadAsset(uuid);
		GetAsset(uuid, [&](Resource* pResource)
		{
			ASSET_DATABASE->m_Callbacks.EnqueueCallback(CallbackType::CT_AssetReloaded, uuid, pResource);
		});
	}

	void AssetManager::UnloadAsset(UUID uuid)
	{
		if (!ASSET_MANAGER->m_pLoadedAssets.Contains(uuid)) return;
		ASSET_MANAGER->m_pLoadedAssets.DoErase(uuid, [](Resource** pResource) { if (*pResource) delete *pResource; });
	}

	Resource* AssetManager::FindResource(UUID uuid)
	{
		if (!ASSET_MANAGER->m_pLoadedAssets.Contains(uuid)) return nullptr;
		return ASSET_MANAGER->m_pLoadedAssets[uuid];
	}

	void AssetManager::AddLoadedResource(Resource* pResource, UUID uuid)
	{
		pResource->m_ID = uuid;
		AddLoadedResource(pResource);
	}

	void AssetManager::AddLoadedResource(Resource* pResource)
	{
		UnloadAsset(pResource->GetUUID());
		ASSET_MANAGER->m_pLoadedAssets.Set(pResource->GetUUID(), pResource);
	}

	bool AssetManager::IsLoading(UUID uuid)
	{
		return ASSET_MANAGER->m_pLoadingAssets.Contains(uuid);
	}

	void AssetManager::GetAllLoading(std::vector<UUID>& out)
	{
		ASSET_MANAGER->m_pLoadingAssets.ForEach([&out](const UUID& uuid) {
			out.push_back(uuid);
		});
	}

	const AssetArchive* AssetManager::GetOrLoadArchive(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			std::stringstream str;
			str << "Failed to load asset archive at path " << path << " file not found!";
			Debug::LogError(str.str());
			return nullptr;
		}

		/* Load as archive */
		BinaryFileStream stream{ path };
		AssetArchive archive{ &stream };
		archive.Deserialize();
		const std::string& str = path.string();
		const uint32_t hash = Hashing::Hash(str.data());
		ASSET_MANAGER->m_LoadedArchives.Emplace(hash, std::move(archive));
		stream.Close();
		return &ASSET_MANAGER->m_LoadedArchives.at(hash);
	}

	void AssetManager::AddAssetArchive(uint32_t hash, AssetArchive&& archive)
	{
		ASSET_MANAGER->m_LoadedArchives.Emplace(hash, std::move(archive));
	}

	bool AssetManager::LoadResourceJob(UUID uuid)
	{
		Resource* pResource = LoadAsset(uuid);
		ASSET_MANAGER->m_ResourceLoadedCallbacks.push(CallbackData(uuid, pResource));
		return pResource;
	}

	Resource* AssetManager::LoadAsset(UUID uuid)
	{
		LoadingLock lock{ uuid };
		if (!lock.IsValid) return nullptr;

		ResourceMeta meta;
		if (!AssetDatabase::GetResourceMeta(uuid, meta)) return nullptr;
		AssetLocation assetLocation;
		if (!AssetDatabase::GetAssetLocation(uuid, assetLocation)) return nullptr;

		if (!assetLocation.SubresourcePath.empty())
		{
			/* Load root resource */
			UUID rootUUID = AssetDatabase::GetAssetUUID(assetLocation.Path);
			Resource* pRootResource = FindResource(rootUUID);
			if (!pRootResource)
				pRootResource = LoadAsset(rootUUID);

			if (!pRootResource) return nullptr;
			Resource* pSubResource = pRootResource->SubresourceFromPath(assetLocation.SubresourcePath);
			if (!pSubResource) return nullptr;

			std::filesystem::path namePath = assetLocation.Path;
			if (!assetLocation.SubresourcePath.empty()) namePath.append(assetLocation.SubresourcePath);
			pSubResource->m_ID = uuid;
			pSubResource->m_Name = meta.Name().empty() ? namePath.string() : meta.Name();
			ASSET_MANAGER->m_pLoadedAssets.Set(uuid, pSubResource);
			ASSET_DATABASE->m_Callbacks.EnqueueCallback(CallbackType::CT_AssetLoaded, uuid, pSubResource);
			return pSubResource;
		}

		Resource* pResource = nullptr;

		std::filesystem::path path = Game::GetAssetPath();
		path.append(assetLocation.Path);
		if (path.extension().compare(".gcag") == 0)
		{
			const AssetArchive* archive = GetOrLoadArchive(path);
			if (archive)
			{
				for (size_t i = 0; i < archive->Size(); ++i)
				{
					Resource* pSubResource = archive->Get(i);
					AddLoadedResource(pResource);
				}

				pResource = FindResource(uuid);
			}
		}

		if (!pResource)
		{
			std::stringstream str;
			str << "Using legacy loading for file: " << path;
			Debug::LogWarning(str.str());
			LoaderModule* pModule = Game::GetGame().GetEngine()->GetLoaderModule(meta.Hash());

			if (pModule == nullptr) return nullptr;

			if (!std::filesystem::exists(path))
				path = assetLocation.Path;

			pResource = pModule->Load(path.string());
		}

		if (pResource == nullptr)
		{
			Debug::LogError("Failed to load asset: " + std::to_string(uuid) + " at path: " + path.string());
			return nullptr;
		}

		std::filesystem::path namePath = assetLocation.Path;
		if (!assetLocation.SubresourcePath.empty()) namePath.append(assetLocation.SubresourcePath);
		pResource->m_ID = uuid;
		pResource->m_Name = meta.Name().empty() ? namePath.string() : meta.Name();
		ASSET_MANAGER->m_pLoadedAssets.Set(uuid, pResource);
		ASSET_DATABASE->m_Callbacks.EnqueueCallback(CallbackType::CT_AssetLoaded, uuid, pResource);
		return pResource;
	}

	AssetManager::AssetManager() : m_pResourceLoadingPool(nullptr) {}

	AssetManager::~AssetManager() {}

	void AssetManager::Initialize()
	{
		AssetDatabase::Initialize();

		ASSET_MANAGER->m_pResourceLoadingPool = Jobs::JobManager::Run<bool, UUID>();
	}

	void AssetManager::Destroy()
	{
		ASSET_MANAGER->m_pLoadedAssets.ForEach([](const UUID& key, Resource* value)
		{
			delete value;
		});

		ASSET_MANAGER->m_LoadedAssetGroups.ForEach([](AssetGroup* value)
		{
			delete value;
		});

		ASSET_MANAGER->m_pLoadedAssets.Clear();
		ASSET_MANAGER->m_LoadedAssetGroups.Clear();
		ASSET_MANAGER->m_PathToGroupIndex.Clear();
		AssetDatabase::Destroy();

		ASSET_MANAGER->m_pResourceLoadingPool = nullptr;
	}

	void AssetManager::RunCallbacks()
	{
		ASSET_DATABASE->m_Callbacks.RunCallbacks();

		CallbackData callbackData;
		while (ASSET_MANAGER->m_ResourceLoadedCallbacks.Pop(callbackData))
		{
			ASSET_MANAGER->m_AssetLoadedCallbacks.Do(callbackData.m_UUID, [&](const std::vector<std::function<void(Resource*)>>& callbacks)
			{
				for (size_t i = 0; i < callbacks.size(); i++)
				{
					callbacks[i](callbackData.m_pResource);
				}
			});
			ASSET_MANAGER->m_AssetLoadedCallbacks.Erase(callbackData.m_UUID);
		}
	}

	CallbackData::CallbackData()
		: m_UUID(0), m_pResource(nullptr)
	{
	}

	CallbackData::CallbackData(UUID uuid, Resource* pResource)
		: m_UUID(uuid), m_pResource(pResource)
	{
	}

	AssetManager::LoadingLock::LoadingLock(UUID uuid) : m_UUID(uuid), IsValid(false)
	{
		if (ASSET_MANAGER->m_pLoadingAssets.Contains(uuid)) return;
		ASSET_MANAGER->m_pLoadingAssets.push_back(uuid);
		IsValid = true;
	}

	AssetManager::LoadingLock::~LoadingLock()
	{
		if (!IsValid) return;
		ASSET_MANAGER->m_pLoadingAssets.Erase(m_UUID);
		IsValid = false;
	}
}
