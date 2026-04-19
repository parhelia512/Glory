#include "RuntimeMaterialManager.h"

#include <IEngine.h>
#include <Resources.h>
#include <MaterialData.h>

namespace Glory
{
	RuntimeMaterialManager::RuntimeMaterialManager(IEngine* pEngine): MaterialManager(pEngine)
	{
	}

	MaterialData* RuntimeMaterialManager::GetMaterial(UUID materialID) const
	{
		Resource* pResource = m_pEngine->GetResources().GetResource(materialID);
		if (!pResource) return nullptr;
		MaterialData* pMaterial = static_cast<MaterialData*>(pResource);
		return pMaterial;
	}

	MaterialData* RuntimeMaterialManager::CreateRuntimeMaterial(UUID baseMaterial)
	{
		Resources& resources = m_pEngine->GetResources();
		Resource* pResource = resources.GetResource(baseMaterial);
		if (!pResource) return nullptr;
		MaterialData* pBaseMaterial = static_cast<MaterialData*>(pResource);
		MaterialData* pMaterialData = pBaseMaterial->CreateCopy();
		m_RuntimeMaterials.push_back(pMaterialData->GetUUID());
		resources.AddResource(&pMaterialData);
		return pMaterialData;
	}

	void RuntimeMaterialManager::DestroyRuntimeMaterials()
	{
		Resources& resources = m_pEngine->GetResources();
		for (auto materialID : m_RuntimeMaterials)
		{
			resources.UnloadResource(materialID);
		}
		m_RuntimeMaterials.clear();
	}
}
