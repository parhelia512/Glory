#include "ProjectSettings.h"
#include "EditorUI.h"
#include "SettingsEnums.h"
#include "ListView.h"
#include "AssetPicker.h"
#include "EditorAssetDatabase.h"
#include "Package.h"
#include "EditorApplication.h"
#include "AssetCompiler.h"

#include <imgui.h>
#include <Reflection.h>
#include <GScene.h>

#include <IconsFontAwesome6.h>

namespace Glory::Editor
{
	std::vector<UUID> Scenes;
	bool FilterScenes = true;

	bool PackageSettings::OnGui()
	{
		ImGui::BeginDisabled(AssetCompiler::IsBusy());
		if (ImGui::Button("Package"))
		{
			StartPackage(EditorApplication::GetInstance()->GetEngine());
		}
		ImGui::EndDisabled();

		static const uint32_t sceneHash = ResourceTypes::GetHash<GScene>();

		ImGui::BeginChild("Packaging Settings");
		bool change = false;

		auto scenePackageMode = m_YAMLFile["Scenes/PackageScenesMode"];
		auto scenesToPackage = m_YAMLFile["Scenes/List"];
		if (!scenesToPackage.Exists())
			scenesToPackage.Set(YAML::Node(YAML::NodeType::Sequence));
		const PackageScenes mode = scenePackageMode.AsEnum<PackageScenes>();
		if (FilterScenes)
		{
			Scenes.clear();
			switch (mode)
			{
			case PackageScenes::All: {
				EditorAssetDatabase::GetAllAssetsOfType(sceneHash, Scenes);
				break;
			}
			case PackageScenes::Opened: {
				break;
			}
			case PackageScenes::List: {
				for (size_t i = 0; i < scenesToPackage.Size(); ++i)
				{
					const UUID sceneID = scenesToPackage[i].As<uint64_t>();
					if (!sceneID) continue;
					Scenes.push_back(sceneID);
				}
				break;
			}
			default:
				break;
			}
			FilterScenes = false;
		}

		if (ImGui::CollapsingHeader("Scene Packaging"))
		{
			if (EditorUI::InputEnum<PackageScenes>(m_YAMLFile, "Scenes/PackageScenesMode"))
			{
				change = true;
				FilterScenes = true;
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip("Which scenes to package:\n\n"
					              "- All will package all scenes in the project,\nthis is the default option\n"
					              "- Opened will package all scenes currently opened\n"
					              "- List will only package the scenes selected in a list"
				);
			}

			if (mode == PackageScenes::List)
			{
				ListView scenesList{"Scenes to Package"};
				scenesList.OnDrawElement = [&scenesToPackage, &change](size_t index) {
					auto scene = scenesToPackage[index];
					UUID sceneID = scene.As<uint64_t>();
					EditorUI::PushFlag(EditorUI::Flag::HasSmallButton);
					if (AssetPicker::ResourceDropdown("Scene", sceneHash, &sceneID, false))
					{
						scene.Set(uint64_t(sceneID));
						FilterScenes = true;
						change = true;
					}
					EditorUI::PopFlag();
				};

				scenesList.OnAdd = [&scenesToPackage]() {
					scenesToPackage.PushBack(uint64_t(0));
				};

				scenesList.OnRemove = [&scenesToPackage](int index) {
					scenesToPackage.Remove(size_t(index));
				};

				if (scenesList.Draw(scenesToPackage.Size()))
				{
					FilterScenes = true;
					change = true;
				}
			}

			ImGui::Spacing();
			ImGui::TextUnformatted("Following scenes will be packaged");
			ImGui::Separator();
			for (size_t i = 0; i < Scenes.size(); ++i)
			{
				const UUID sceneID = Scenes[i];
				const std::string name = EditorAssetDatabase::GetAssetName(sceneID);
				ImGui::Selectable(name.data());
			}
			ImGui::Separator();

			auto entryScene = m_YAMLFile["Scenes/EntryScene"];
			if (!entryScene.Exists())
				entryScene.Set(0);

			UUID entrySceneID = entryScene.As<uint64_t>();
			if (entrySceneID == 0 && !Scenes.empty())
			{
				entrySceneID = Scenes[0];
				entryScene.Set(uint64_t(entrySceneID));
			}

			if (AssetPicker::ResourceDropdown("Entry Scene", sceneHash, &entrySceneID, false))
			{
				entryScene.Set(uint64_t(entrySceneID));
				FilterScenes = true;
				change = true;
			}

			const float childHeight = ImGui::CalcTextSize("A").y * 3;
			const auto itor = std::find(Scenes.begin(), Scenes.end(), entrySceneID);
			if (entrySceneID == 0)
			{
				ImGui::BeginChild("error", { 0.0f, childHeight }, true, ImGuiWindowFlags_MenuBar);
				if (ImGui::BeginMenuBar())
				{
					ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, ICON_FA_CIRCLE_EXCLAMATION);
					ImGui::SameLine();
					ImGui::Text(" Error");
					ImGui::EndMenuBar();
				}
				ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "ERROR: %s", "No entry scene set");
				ImGui::EndChild();
			}
			else if (itor == Scenes.end())
			{
				ImGui::BeginChild("error", { 0.0f, childHeight }, true, ImGuiWindowFlags_MenuBar);
				if (ImGui::BeginMenuBar())
				{
					ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, ICON_FA_CIRCLE_EXCLAMATION);
					ImGui::SameLine();
					ImGui::Text(" Error");
					ImGui::EndMenuBar();
				}
				ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "ERROR: %s", "Chosen entry scene is not selected to be packaged");
				ImGui::EndChild();
			}
		}

		auto alwaysPackageAssets = m_YAMLFile["Assets/ForcePackageList"];
		if (!alwaysPackageAssets.Exists())
			alwaysPackageAssets.Set(YAML::Node(YAML::NodeType::Sequence));

		if (ImGui::CollapsingHeader("Asset Packaging"))
		{
			ListView assetsList{ "Assets to Always Package" };
			assetsList.OnDrawElement = [&alwaysPackageAssets, &change](size_t index) {
				auto asset = alwaysPackageAssets[index];
				UUID assetID = asset.As<uint64_t>();
				EditorUI::PushFlag(EditorUI::Flag::HasSmallButton);
				if (AssetPicker::ResourceDropdown("Scene", 0, &assetID, false))
				{
					asset.Set(uint64_t(assetID));
					change = true;
				}
				EditorUI::PopFlag();
				};

			assetsList.OnAdd = [&alwaysPackageAssets]() {
				alwaysPackageAssets.PushBack(uint64_t(0));
				};

			assetsList.OnRemove = [&alwaysPackageAssets](int index) {
				alwaysPackageAssets.Remove(size_t(index));
				};

			change |= assetsList.Draw(alwaysPackageAssets.Size());
		}

		ImGui::EndChild();
		return change;
	}

	void PackageSettings::OnSettingsLoaded()
	{
		VerifySettings();
	}

	void PackageSettings::VerifySettings()
	{
		auto packageScenesMode = RootValue()["Scenes/PackageScenesMode"];
		if (!packageScenesMode.Exists())
		{
			packageScenesMode.SetEnum(PackageScenes::All);
		}
		auto scenesToPackage = m_YAMLFile["Scenes/List"];
		if (!scenesToPackage.Exists())
			scenesToPackage.Set(YAML::Node(YAML::NodeType::Sequence));
		auto entryScene = m_YAMLFile["Scenes/EntryScene"];
		if (!entryScene.Exists())
		{
			entryScene.Set(0);
			const PackageScenes mode = packageScenesMode.AsEnum<PackageScenes>();

			Scenes.clear();
			switch (mode)
			{
			case PackageScenes::All: {
				std::vector<UUID> scenes;
				static const uint32_t sceneHash = ResourceTypes::GetHash<GScene>();
				EditorAssetDatabase::GetAllAssetsOfType(sceneHash, scenes);
				if (!scenes.empty())
					entryScene.Set(uint64_t(scenes[0]));
				break;
			}
			case PackageScenes::Opened: {
				break;
			}
			case PackageScenes::List: {
				if (scenesToPackage.Size() > 0)
				{
					const UUID sceneID = scenesToPackage[0].As<uint64_t>();
					entryScene.Set(uint64_t(sceneID));
				}
				break;
			}
			default:
				break;
			}
		}
	}
}
