#include "MaterialInstanceData.h"
#include "ResourceType.h"
#include "BinaryStream.h"
#include "AssetManager.h"

#include <algorithm>

namespace Glory
{
	MaterialInstanceData::MaterialInstanceData() : m_pBaseMaterial(nullptr)
	{
	}

	MaterialInstanceData::MaterialInstanceData(MaterialData* pBaseMaterial) : m_pBaseMaterial(pBaseMaterial)
	{
		APPEND_TYPE(MaterialInstanceData);

		if (!pBaseMaterial) return;
		m_PropertyOverridesEnable.resize(pBaseMaterial->PropertyInfoCount(), false);
		m_PropertyBuffer.resize(pBaseMaterial->GetBufferReference().size());
		m_Resources.resize(pBaseMaterial->ResourceCount());
	}

	MaterialInstanceData::~MaterialInstanceData()
	{
		m_pBaseMaterial = nullptr;
	}

	size_t MaterialInstanceData::ShaderCount() const
	{
		if (!m_pBaseMaterial) return 0;
		return m_pBaseMaterial->ShaderCount();
	}

	ShaderSourceData* MaterialInstanceData::GetShaderAt(size_t index) const
	{
		return m_pBaseMaterial->GetShaderAt(index);
	}

	const ShaderType& MaterialInstanceData::GetShaderTypeAt(size_t index) const
	{
		return m_pBaseMaterial->GetShaderTypeAt(index);
	}

	MaterialData* MaterialInstanceData::GetBaseMaterial() const
	{
		return m_pBaseMaterial;
	}

	void MaterialInstanceData::SetBaseMaterial(MaterialData* pMaterial)
	{
		m_pBaseMaterial = pMaterial;

		if (m_pBaseMaterial == nullptr)
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
		if (!m_pBaseMaterial) return 0;
		return m_pBaseMaterial->GetUUID();
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
		if (!m_pBaseMaterial) return 0;
		return m_pBaseMaterial->PropertyInfoCount();
	}

	MaterialPropertyInfo* MaterialInstanceData::GetPropertyInfoAt(size_t index)
	{
		return m_pBaseMaterial->GetPropertyInfoAt(index);
	}

	size_t MaterialInstanceData::GetCurrentBufferOffset() const
	{
		if (!m_pBaseMaterial) return 0;
		return m_pBaseMaterial->GetCurrentBufferOffset();
	}

	std::vector<char>& MaterialInstanceData::GetBufferReference()
	{
		if (!m_pBaseMaterial) return m_PropertyBuffer;
		std::vector<char>& baseBuffer = m_pBaseMaterial->GetBufferReference();
		m_PropertyBuffer.resize(baseBuffer.size());
		for (size_t i = 0; i < m_pBaseMaterial->PropertyInfoCount(); i++)
		{
			MaterialPropertyInfo* propertyInfo = m_pBaseMaterial->GetPropertyInfoAt(i);
			if (propertyInfo->IsResource()) continue;
			if (m_PropertyOverridesEnable[i]) continue;
			memcpy(&m_PropertyBuffer[propertyInfo->Offset()], &baseBuffer[propertyInfo->Offset()], propertyInfo->Size());
		}
		return m_PropertyBuffer;
	}

	bool MaterialInstanceData::GetPropertyInfoIndex(const std::string& name, size_t& index) const
	{
		return m_pBaseMaterial ? m_pBaseMaterial->GetPropertyInfoIndex(name, index) : false;
	}

	AssetReference<TextureData>* MaterialInstanceData::GetResourceUUIDPointer(size_t index)
	{
		size_t propertyIndex = GetPropertyIndexFromResourceIndex(index);
		return m_PropertyOverridesEnable[propertyIndex] ? &m_Resources[index] : m_pBaseMaterial->GetResourceUUIDPointer(index);
	}

	size_t MaterialInstanceData::GetPropertyIndexFromResourceIndex(size_t index) const
	{
		return m_pBaseMaterial->GetPropertyIndexFromResourceIndex(index);
	}

	size_t MaterialInstanceData::GetResourcePropertyCount() const
	{
		return m_pBaseMaterial->GetResourcePropertyCount();
	}

	MaterialPropertyInfo* MaterialInstanceData::GetResourcePropertyInfo(size_t index)
	{
		return m_pBaseMaterial->GetResourcePropertyInfo(index);
	}

	void MaterialInstanceData::ReloadProperties()
	{
		if (!m_pBaseMaterial || m_PropertyBuffer.size() == m_pBaseMaterial->GetBufferReference().size()) return;
		m_PropertyOverridesEnable.resize(m_pBaseMaterial->PropertyInfoCount(), false);
		m_PropertyBuffer.resize(m_pBaseMaterial->GetBufferReference().size());
		m_Resources.resize(m_pBaseMaterial->ResourceCount());
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
		container.Write(m_pBaseMaterial->GetUUID());

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
		UUID baseMaterialID;
		container.Read(baseMaterialID);
		m_pBaseMaterial = AssetManager::GetAssetImmediate<MaterialData>(baseMaterialID);

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
		if (!m_PropertyOverridesEnable[index]) return m_pBaseMaterial->m_PropertyBuffer;
		return m_PropertyBuffer;
	}
}
