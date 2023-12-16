#include "MaterialInstanceData.h"
#include "ResourceType.h"
#include "BinaryStream.h"

#include <algorithm>

namespace Glory
{
	MaterialInstanceData::MaterialInstanceData() : m_BaseMaterialID(0)
	{
		APPEND_TYPE(MaterialInstanceData);
	}

	MaterialInstanceData::MaterialInstanceData(UUID uuid, const std::string& name): MaterialData(uuid, name), m_BaseMaterialID(0)
	{
		APPEND_TYPE(MaterialInstanceData);
	}

	MaterialInstanceData::MaterialInstanceData(MaterialData* pBaseMaterial) : m_BaseMaterialID(pBaseMaterial->GetUUID())
	{
		APPEND_TYPE(MaterialInstanceData);

		if (!pBaseMaterial) return;
		m_PropertyOverridesEnable.resize(pBaseMaterial->PropertyInfoCount(), false);
		m_PropertyBuffer.resize(pBaseMaterial->GetBufferReference().size());
		m_Resources.resize(pBaseMaterial->ResourceCount());
	}

	MaterialInstanceData::~MaterialInstanceData()
	{
		m_BaseMaterialID = 0;
	}

	size_t MaterialInstanceData::ShaderCount() const
	{
		if (!m_BaseMaterialID) return 0;
		MaterialData* pBaseMaterial = Game::GetGame().GetEngine()->GetResources().Manager<MaterialData>()->Get(m_BaseMaterialID);
		return pBaseMaterial ? pBaseMaterial->ShaderCount() : 0;
	}

	UUID MaterialInstanceData::GetShaderAt(size_t index) const
	{
		if (!m_BaseMaterialID) return 0;
		MaterialData* pBaseMaterial = Game::GetGame().GetEngine()->GetResources().Manager<MaterialData>()->Get(m_BaseMaterialID);
		return pBaseMaterial ? pBaseMaterial->GetShaderAt(index) : 0;
	}

	MaterialData* MaterialInstanceData::GetBaseMaterial() const
	{
		return Game::GetGame().GetEngine()->GetResources().Manager<MaterialData>()->Get(m_BaseMaterialID);
	}

	void MaterialInstanceData::SetBaseMaterial(MaterialData* pMaterial)
	{
		m_BaseMaterialID = pMaterial->GetUUID();

		if (!m_BaseMaterialID)
		{
			m_PropertyOverridesEnable.clear();
			m_PropertyBuffer.clear();
			m_Resources.clear();
			return;
		}

		ReloadProperties();
	}

	UUID MaterialInstanceData::GetGPUUUID() const
	{
		return m_BaseMaterialID;
	}

	void MaterialInstanceData::CopyOverrideStates(std::vector<bool>& destination)
	{
		std::unique_lock lock(m_PropertiesAccessMutex);
		if (m_PropertyOverridesEnable.size() != destination.size()) destination.resize(m_PropertyOverridesEnable.size());
		destination.assign(m_PropertyOverridesEnable.begin(), m_PropertyOverridesEnable.end());
		lock.unlock();
	}

	void MaterialInstanceData::PasteOverrideStates(std::vector<bool>& destination)
	{
		std::unique_lock lock(m_PropertiesAccessMutex);
		if (m_PropertyOverridesEnable.size() != destination.size()) m_PropertyOverridesEnable.resize(destination.size());
		m_PropertyOverridesEnable.assign(destination.begin(), destination.end());
		lock.unlock();
	}

	size_t MaterialInstanceData::PropertyInfoCount() const
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		if (!pBaseMaterial) return 0;
		return pBaseMaterial->PropertyInfoCount();
	}

	MaterialPropertyInfo* MaterialInstanceData::GetPropertyInfoAt(size_t index)
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		if (!pBaseMaterial) return nullptr;
		return pBaseMaterial->GetPropertyInfoAt(index);
	}

	size_t MaterialInstanceData::GetCurrentBufferOffset() const
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		if (!pBaseMaterial) return 0;
		return pBaseMaterial->GetCurrentBufferOffset();
	}

	std::vector<char>& MaterialInstanceData::GetBufferReference()
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		if (!pBaseMaterial) return m_PropertyBuffer;
		std::vector<char>& baseBuffer = pBaseMaterial->GetBufferReference();
		m_PropertyBuffer.resize(baseBuffer.size());
		for (size_t i = 0; i < pBaseMaterial->PropertyInfoCount(); i++)
		{
			MaterialPropertyInfo* propertyInfo = pBaseMaterial->GetPropertyInfoAt(i);
			if (propertyInfo->IsResource()) continue;
			if (m_PropertyOverridesEnable[i]) continue;
			memcpy(&m_PropertyBuffer[propertyInfo->Offset()], &baseBuffer[propertyInfo->Offset()], propertyInfo->Size());
		}
		return m_PropertyBuffer;
	}

	bool MaterialInstanceData::GetPropertyInfoIndex(const std::string& name, size_t& index) const
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		return pBaseMaterial ? pBaseMaterial->GetPropertyInfoIndex(name, index) : false;
	}

	AssetReference<TextureData>* MaterialInstanceData::GetResourceUUIDPointer(size_t index)
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		size_t propertyIndex = GetPropertyIndexFromResourceIndex(index);
		return m_PropertyOverridesEnable[propertyIndex] ? &m_Resources[index] : pBaseMaterial->GetResourceUUIDPointer(index);
	}

	size_t MaterialInstanceData::GetPropertyIndexFromResourceIndex(size_t index) const
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		return pBaseMaterial->GetPropertyIndexFromResourceIndex(index);
	}

	size_t MaterialInstanceData::GetResourcePropertyCount() const
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		return pBaseMaterial->GetResourcePropertyCount();
	}

	MaterialPropertyInfo* MaterialInstanceData::GetResourcePropertyInfo(size_t index)
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		return pBaseMaterial->GetResourcePropertyInfo(index);
	}

	void MaterialInstanceData::ReloadProperties()
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		if (!pBaseMaterial || m_PropertyBuffer.size() == pBaseMaterial->GetBufferReference().size()) return;
		m_PropertyOverridesEnable.resize(pBaseMaterial->PropertyInfoCount(), false);
		m_PropertyBuffer.resize(pBaseMaterial->GetBufferReference().size());
		m_Resources.resize(pBaseMaterial->ResourceCount());
	}

	bool MaterialInstanceData::IsPropertyOverriden(size_t index) const
	{
		return m_PropertyOverridesEnable[index];
	}

	void MaterialInstanceData::EnableProperty(size_t index)
	{
		m_PropertyOverridesEnable[index] = true;
	}

	void MaterialInstanceData::Serialize(BinaryStream& container) const
	{
		/* Write base material */
		container.Write(m_BaseMaterialID);

		/* Write overrides */
		container.Write(m_PropertyOverridesEnable.size());
		for (size_t i = 0; i < m_PropertyOverridesEnable.size(); ++i)
		{
			container.Write(m_PropertyOverridesEnable[i]);
		}

		/* Write property buffer */
		container.Write(m_PropertyBuffer.size()).
			Write(m_PropertyBuffer.data(), m_PropertyBuffer.size());

		/* Write resources */
		container.Write(m_Resources.size());
		for (size_t i = 0; i < m_Resources.size(); ++i)
		{
			container.Write(m_Resources[i].AssetUUID());
		}
	}

	void MaterialInstanceData::Deserialize(BinaryStream& container)
	{
		/* Read base material */
		container.Read(m_BaseMaterialID);

		/* Write overrides */
		size_t numPropertyOverrides;
		container.Read(numPropertyOverrides);
		m_PropertyOverridesEnable.resize(numPropertyOverrides);
		for (size_t i = 0; i < m_PropertyOverridesEnable.size(); ++i)
		{
			container.Read(m_PropertyOverridesEnable[i]);
		}

		/* Read property buffer */
		size_t propertyBufferSize;
		container.Read(propertyBufferSize);
		m_PropertyBuffer.resize(propertyBufferSize);
		container.Read(m_PropertyBuffer.data(), propertyBufferSize);

		/* Read resources */
		size_t numResources;
		container.Read(numResources);
		m_Resources.resize(numResources);
		for (size_t i = 0; i < m_Resources.size(); ++i)
		{
			container.Read(*m_Resources[i].AssetUUIDMember());
		}
	}

	std::vector<char>& MaterialInstanceData::GetPropertyBuffer(size_t index)
	{
		MaterialData* pBaseMaterial = GetBaseMaterial();
		if (!m_PropertyOverridesEnable[index]) return pBaseMaterial->m_PropertyBuffer;
		return m_PropertyBuffer;
	}
}
