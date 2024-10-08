#include "Material.h"
#include "Engine.h"
#include "AssetManager.h"
#include "GraphicsModule.h"

namespace Glory
{
	Buffer* Material::m_pMVPBuffer = nullptr;

	Material::Material(MaterialData* pMaterialData)
		: m_pMaterialData(pMaterialData), m_pPropertiesBuffer(nullptr), m_Complete(false), m_pPipeline(nullptr)
	{
	}

	Material::~Material()
	{
	}

	void Material::SetProperties(Engine* pEngine)
	{
		if (m_pMaterialData == nullptr) return;

		for (size_t i = 0; i < m_pMaterialData->GetResourcePropertyCount(pEngine->GetMaterialManager()); i++)
		{
			MaterialPropertyInfo* pPropertyInfo = m_pMaterialData->GetResourcePropertyInfo(pEngine->GetMaterialManager(), i);
			const std::string& shaderName = pPropertyInfo->ShaderName();
			const UUID uuid = m_pMaterialData->GetResourceUUIDPointer(pEngine->GetMaterialManager(), i)->AssetUUID();
			TextureData* pTextureData = pEngine->GetAssetManager().GetOrLoadAsset<TextureData>(uuid);
			Texture* pTexture = pTextureData != nullptr ? pEngine->GetMainModule<GraphicsModule>()->GetResourceManager()->CreateTexture(pTextureData) : nullptr;
			SetTexture(shaderName, pTexture);
		}

		std::vector<char>& propertyBuffer = m_pMaterialData->GetBufferReference(pEngine->GetMaterialManager());
		if (propertyBuffer.empty()) return;
		if (m_pPropertiesBuffer == nullptr) m_pPropertiesBuffer = CreatePropertiesBuffer((uint32_t)propertyBuffer.size());
		m_pPropertiesBuffer->Assign((const void*)propertyBuffer.data());
		m_pPropertiesBuffer->BindForDraw();
	}

	void Material::SetObjectData(const ObjectData& data)
	{
		if (m_pMVPBuffer == nullptr)
			m_pMVPBuffer = CreateMVPBuffer();

		m_pMVPBuffer->Assign((const void*)&data);
		m_pMVPBuffer->BindForDraw();
	}
}
