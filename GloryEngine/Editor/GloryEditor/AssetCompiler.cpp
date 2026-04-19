#include "AssetCompiler.h"
#include "EditorAssetDatabase.h"
#include "EditorApplication.h"
#include "EditorSceneManager.h"
#include "EditorPipelineManager.h"
#include "EditorAssetManager.h"
#include "RemovedAssetsPopup.h"
#include "ThumbnailManager.h"
#include "Dispatcher.h"
#include "Importer.h"
#include "ProjectSpace.h"

#include <Debug.h>
#include <AssetDatabase.h>
#include <JobManager.h>
#include <Resources.h>
#include <BinaryStream.h>
#include <PipelineData.h>
#include <AssetArchive.h>

namespace Glory::Editor
{
	std::map<UUID, AssetCompiler::AssetData> AssetCompiler::m_AssetDatas;
	ThreadedVector<UUID> AssetCompiler::m_CompilingAssets;
	ThreadedVector<UUID> AssetCompiler::m_ToRemoveAssets;
	ThreadedUMap<std::filesystem::path, ImportedResource> ImportedResources;
	ThreadedVector<UUID> AssetCompiler::m_CompletedAssets;

	void AssetCompiler::CompilePipelines()
	{
		std::vector<UUID> ids;
		EditorAssetDatabase::GetAllAssetsOfType(ResourceTypes::GetHash<PipelineData>(), ids);
		//CompileAssetsImmediately(ids);

		throw "Get back here!";
	}

	void AssetCompiler::CompileAssetDatabase()
	{
		CompileAssetDatabase(EditorAssetDatabase::UUIDs());
	}

	void AssetCompiler::CompileAssetDatabase(UUID id)
	{
		CompileAssetDatabase(std::vector<UUID>{ id });
	}

	void AssetCompiler::CompileAssetDatabase(const std::vector<UUID>& ids)
	{
		IEngine* pEngine = EditorApplication::GetInstance()->GetEngine();
		AssetDatabase& assetDatabase = pEngine->GetAssetDatabase();
		Resources& assetManager = pEngine->GetResources();
		AssetDatabase::WriteLock lock{ &assetDatabase };

		for (UUID id : ids)
		{
			AssetData& data = m_AssetDatas[id];

			if (!EditorAssetDatabase::GetAssetLocation(id, data.Location))
			{
				assetDatabase.Remove(id);
				m_AssetDatas.erase(id);
				continue;
			}

			if (!EditorAssetDatabase::GetAssetMetadata(id, data.Meta))
			{
				assetDatabase.Remove(id);
				m_AssetDatas.erase(id);
				continue;
			}

			AssetLocation location = data.Location;
			location.Index = 0;
			location.Path = GenerateCompiledResourcePath(id).string();
			assetDatabase.SetAsset(location, data.Meta);
		}
	}

	bool AssetCompiler::IsCompilingAsset(UUID uuid)
	{
		return m_CompilingAssets.Contains(uuid);
	}

	bool AssetCompiler::CompileSceneSettings(UUID uuid)
	{
		EditorSceneManager& sceneManager = EditorApplication::GetInstance()->GetSceneManager();
		GScene* pScene = sceneManager.GetOpenScene(uuid);
		if (!pScene) return false;
		auto sceneFile = sceneManager.GetSceneFile(uuid);
		return CompileSceneSettings(pScene, **sceneFile);
	}

	bool AssetCompiler::CompileSceneSettings(GScene* pScene, Utils::NodeValueRef root)
	{
		if (!pScene) return false;

		SceneSettings& sceneSettings = pScene->Settings();
		auto settings = root["Settings"];
		if (!settings.Exists() || !settings.IsMap())
			return false;

		auto rendering = settings["Rendering"];
		if (!rendering.Exists() || !rendering.IsMap())
			return false;

		auto lighting = rendering["Lighting"];
		if (!lighting.Exists() || !lighting.IsMap())
			lighting.SetMap();

		auto skybox = lighting["Skybox"];
		sceneSettings.m_LightingSettings.m_Skybox = skybox.As<uint64_t>(0);
		auto irradiance = lighting["Irradiance"];
		sceneSettings.m_LightingSettings.m_IrradianceMap = irradiance.As<uint64_t>(0);

		auto ssao = rendering["SSAO"];
		if (!ssao.Exists() || !ssao.IsMap())
			ssao.SetMap();

		auto enable = ssao["Enable"];
		auto sampleRadius = ssao["SampleRadius"];
		auto sampleBias = ssao["SampleBias"];
		auto kernelSize = ssao["KernelSize"];
		auto blurType = ssao["BlurType"];
		auto blurSize = ssao["BlurSize"];
		auto separation = ssao["Separation"];
		auto binsSize = ssao["BinsSize"];
		auto magnitude = ssao["Magnitude"];
		auto contrast = ssao["Contrast"];

		sceneSettings.m_SSAOSettings.m_Enabled = enable.As<bool>(DefaultSSAO.m_Enabled);
		sceneSettings.m_SSAOSettings.m_SampleRadius = sampleRadius.As<float>(DefaultSSAO.m_SampleRadius);
		sceneSettings.m_SSAOSettings.m_SampleBias = sampleBias.As<float>(DefaultSSAO.m_SampleBias);
		sceneSettings.m_SSAOSettings.m_KernelSize = kernelSize.As<int>(DefaultSSAO.m_KernelSize);
		sceneSettings.m_SSAOSettings.m_BlurType = blurType.AsEnum<BlurType>(DefaultSSAO.m_BlurType);
		sceneSettings.m_SSAOSettings.m_BlurSize = blurSize.As<int>(DefaultSSAO.m_BlurSize);
		sceneSettings.m_SSAOSettings.m_Separation = separation.As<float>(DefaultSSAO.m_Separation);
		sceneSettings.m_SSAOSettings.m_BinsSize = binsSize.As<int>(DefaultSSAO.m_BinsSize);
		sceneSettings.m_SSAOSettings.m_Magnitude = magnitude.As<float>(DefaultSSAO.m_Magnitude);
		sceneSettings.m_SSAOSettings.m_Contrast = contrast.As<float>(DefaultSSAO.m_Contrast);
		sceneSettings.m_SSAOSettings.m_Dirty = true;
		return true;
	}

	void AssetCompiler::RemoveDeletedAssets()
	{
		if (IsBusy()) return;
		std::vector<UUID> removed{};
		std::vector<AssetLocation> removedLocations{};
		removed.reserve(m_ToRemoveAssets.Size());
		m_ToRemoveAssets.ForEachClear([&removed, &removedLocations](UUID uuid) {
			EditorAssetDatabase::RemoveAsset(uuid, false);
			removed.push_back(uuid);
			removedLocations.push_back(m_AssetDatas.at(uuid).Location);
		});
		if (removed.empty()) return;
		CompileAssetDatabase(removed);
		RemovedAssetsPopup::AddRemovedAssets(std::move(removedLocations));
	}

	void AssetCompiler::Update()
	{
		m_CompletedAssets.ForEachClear([](UUID& uuid) {
			EditorApplication::GetInstance()->GetThumbnailManager().SetDirty(uuid);
			GetAssetCompilerEventDispatcher().Dispatch({ uuid });
		});
	}

	AssetCompilerEventDispatcher& AssetCompiler::GetAssetCompilerEventDispatcher()
	{
		static AssetCompilerEventDispatcher dispatcher;
		return dispatcher;
	}

	std::filesystem::path AssetCompiler::GenerateCompiledResourcePath(const UUID uuid)
	{
		std::filesystem::path compiledPath = ProjectSpace::GetOpenProject()->CachePath();
		compiledPath.append("CompiledResources").append(std::to_string(uuid)).replace_extension(".gcag");
		return compiledPath;
	}
}
