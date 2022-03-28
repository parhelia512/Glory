#pragma once
#include "ResourceLoaderModule.h"
#include "MaterialData.h"
#include "MaterialInstanceData.h"

namespace Glory
{
	struct MaterialImportSettings : public ImportSettings
	{
		MaterialImportSettings();
		MaterialImportSettings(const std::string& name);
	};

	class MaterialLoaderModule : public ResourceLoaderModule<MaterialData, MaterialImportSettings>
	{
	public:
		MaterialLoaderModule();
		virtual ~MaterialLoaderModule();

		virtual const std::type_info& GetModuleType() override;

	protected:
		virtual MaterialImportSettings ReadImportSettings_Internal(YAML::Node& node) override;
		virtual void WriteImportSettings_Internal(const MaterialImportSettings& importSettings, YAML::Emitter& out) override;

	private:
		virtual MaterialData* LoadResource(const std::string& path, const MaterialImportSettings& importSettings) override;
		virtual MaterialData* LoadResource(const void* buffer, size_t length, const MaterialImportSettings& importSettings) override;

	private:
		MaterialData* LoadMaterialData(YAML::Node& rootNode, const MaterialImportSettings& importSettings);
		MaterialInstanceData* LoadMaterialInstanceData(YAML::Node& rootNode, const MaterialImportSettings& importSettings);

		void ReadShaders(YAML::Node& rootNode, MaterialData* pMaterialData);
		void ReadPropertyData(YAML::Node& rootNode, MaterialData* pMaterialData);
		void ReadPropertyOverrides(YAML::Node& rootNode, MaterialInstanceData* pMaterialData);

	protected:
		virtual void Initialize() override;
		virtual void Cleanup() override;
	};
}