#include "MaterialImporter.h"
#include "EditorAssetDatabase.h"
#include "EditorApplication.h"
#include "EditorMaterialManager.h"
#include "EditorResourceManager.h"
#include "EditorPipelineManager.h"

#include <fstream>
#include <Resources.h>
#include <AssetDatabase.h>
#include <PipelineData.h>
#include <PropertySerializer.h>

namespace Glory::Editor
{
	MaterialImporter::MaterialImporter()
	{
	}

	MaterialImporter::~MaterialImporter()
	{
	}

	std::string_view MaterialImporter::Name() const
	{
		return "Internal Material Importer";
	}

	bool MaterialImporter::SupportsExtension(const std::filesystem::path& extension) const
	{
		return extension.compare(".gmat") == 0 || extension.compare(".gminst") == 0;
	}

	ImportedResource MaterialImporter::LoadResource(const std::filesystem::path& path, void*) const
	{
		MaterialData* pNewMaterialData = new MaterialData();
		EditorPipelineManager& pipelines = EditorApplication::GetInstance()->GetPipelineManager();
		EditorMaterialManager& materials = EditorApplication::GetInstance()->GetMaterialManager();

		const UUID materialID = EditorAssetDatabase::FindAssetUUID(path.string());
		if (materialID)
		{
			YAMLResource<MaterialData>* pMaterialData = static_cast<YAMLResource<MaterialData>*>(
				EditorApplication::GetInstance()->GetResourceManager().GetEditableResource(materialID));
			Utils::NodeValueRef node = **pMaterialData;
			const UUID pipelineID = node["Pipeline"].As<uint64_t>(0ull);
			if(!pipelineID) return { path, pNewMaterialData };
			pNewMaterialData->SetPipeline(pipelineID);
			PipelineData* pPipeline = pipelines.GetPipelineData(pipelineID);
			if (pPipeline)
			{
				pPipeline->LoadIntoMaterial(pNewMaterialData);
				materials.ReadPropertiesInto(node["Properties"], pNewMaterialData);
			}
			return { path, pNewMaterialData };
		}

		Utils::YAMLFileRef file{ path };
		const UUID pipelineID = file["Pipeline"].As<uint64_t>(0ull);
		if (!pipelineID) return { path, pNewMaterialData };
		pNewMaterialData->SetPipeline(pipelineID);
		PipelineData* pPipeline = pipelines.GetPipelineData(pipelineID);
		if (pPipeline)
		{
			pPipeline->LoadIntoMaterial(pNewMaterialData);
			materials.ReadPropertiesInto(file["Properties"], pNewMaterialData);
		}
		return ImportedResource{ path, pNewMaterialData };
	}

	bool MaterialImporter::SaveResource(const std::filesystem::path& path, MaterialData* pResource) const
	{
		Utils::YAMLFileRef file{ path };
		SaveMaterialData(pResource, file.RootNodeRef().ValueRef());
		file.Save();
		return true;
	}

	void MaterialImporter::SaveMaterialData(MaterialData* pMaterialData, Utils::NodeValueRef data) const
	{
		data.SetMap();
		data["Pipeline"].Set(uint64_t(pMaterialData->GetPipelineID()));
		WritePropertyData(data, pMaterialData);
	}

	void MaterialImporter::WritePropertyData(Utils::NodeValueRef data, MaterialData* pMaterialData) const
	{
		EditorMaterialManager& manager = EditorApplication::GetInstance()->GetMaterialManager();

		auto properties = data["Properties"];

		size_t resourceIndex = 0;
		for (size_t i = 0; i < pMaterialData->PropertyInfoCount(); i++)
		{
			properties.PushBack(YAML::Node(YAML::NodeType::Map));
			auto property = properties[i];

			MaterialPropertyInfo* pPropertyInfo = pMaterialData->GetPropertyInfoAt(i);
			property["DisplayName"].Set(pPropertyInfo->DisplayName());
			property["ShaderName"].Set(pPropertyInfo->ShaderName());
			property["TypeHash"].Set(pPropertyInfo->TypeHash());

			bool isResource = EditorApplication::GetInstance()->GetEngine()->GetResourceTypes().IsResource(pPropertyInfo->TypeHash());
			if (!isResource)
			{
				EditorApplication::GetInstance()->GetSerializers().SerializeProperty(pMaterialData->GetBufferReference(), pPropertyInfo->TypeHash(), pPropertyInfo->Offset(), pPropertyInfo->Size(), property["Value"]);
			}
			else
			{
				size_t index = pMaterialData->GetPropertyIndexFromResourceIndex(resourceIndex);
				++resourceIndex;
				const uint64_t uuid = pMaterialData->GetResourceUUIDPointer(index)->GetUUID();
				property["Value"].Set(uuid);
			}
		}
	}

	void MaterialImporter::Initialize()
	{
	}

	void MaterialImporter::Cleanup()
	{
	}
}
