#include "MaterialData.h"
#include "AssetManager.h"
#include "BinaryStream.h"
#include "PipelineData.h"
#include "PipelineManager.h"

#include <algorithm>

namespace Glory
{
	MaterialData::MaterialData()
		: m_Pipeline(0), m_CurrentOffset(0)
	{
		APPEND_TYPE(MaterialData);
	}

	MaterialData::~MaterialData()
	{
	}

	void MaterialData::AddProperty(const std::string& displayName, const std::string& shaderName, uint32_t typeHash, size_t size, bool isResource, uint32_t flags)
	{
		const uint32_t hash = Reflect::Hash(displayName.data());
		if (m_HashToPropertyInfoIndex.find(hash) != m_HashToPropertyInfoIndex.end())
			return;

		const size_t index = m_PropertyInfos.size();
		size_t lastIndex = index - 1;
		m_PropertyInfos.emplace_back(MaterialPropertyInfo(displayName, shaderName, typeHash, size, m_CurrentOffset, flags));
		m_CurrentOffset = m_PropertyInfos[index].EndOffset();
		m_PropertyInfos[index].Reserve(m_PropertyBuffer);
		m_HashToPropertyInfoIndex[hash] = index;
	}

	void MaterialData::AddProperty(const std::string& displayName, const std::string& shaderName, uint32_t typeHash, UUID resourceUUID, uint32_t flags)
	{
		const uint32_t hash = Reflect::Hash(displayName.data());
		if (m_HashToPropertyInfoIndex.find(hash) != m_HashToPropertyInfoIndex.end())
		{
			const size_t index = m_HashToPropertyInfoIndex.at(hash);
			const size_t resourceOffset = m_PropertyInfos[index].Offset();
			m_Resources[resourceOffset] = resourceUUID;
			return;
		}

		const size_t index = m_PropertyInfos.size();
		m_PropertyInfos.push_back(MaterialPropertyInfo(displayName, shaderName, typeHash, m_Resources.size(), flags));
		m_ResourcePropertyInfoIndices.push_back(index);
		m_HashToPropertyInfoIndex[hash] = index;
		m_Resources.push_back(resourceUUID);
	}

	void MaterialData::AddProperty(const MaterialPropertyInfo& other)
	{
		const uint32_t hash = Reflect::Hash(other.DisplayName().data());
		if (m_HashToPropertyInfoIndex.find(hash) != m_HashToPropertyInfoIndex.end())
			return;

		const size_t index = m_PropertyInfos.size();
		m_PropertyInfos.push_back(MaterialPropertyInfo(other));
		if (other.IsResource())
		{
			m_ResourcePropertyInfoIndices.push_back(index);
			m_Resources.push_back(0);
		}
		else
		{
			m_PropertyInfos[index].Reserve(m_PropertyBuffer);
		}
		m_HashToPropertyInfoIndex[hash] = index;
	}

	void MaterialData::SetPipeline(PipelineData* pPipeline)
	{
		m_Pipeline = pPipeline->GetUUID();
	}

	void MaterialData::SetPipeline(UUID pipelineID)
	{
		m_Pipeline = pipelineID;
	}

	PipelineData* MaterialData::GetPipeline(const MaterialManager& manager, const PipelineManager& pipelineManager) const
	{
		return m_Pipeline ? pipelineManager.GetPipelineData(m_Pipeline) : nullptr;
	}

	UUID MaterialData::GetPipelineID(const MaterialManager&) const
	{
		return m_Pipeline;
	}

	size_t MaterialData::PropertyInfoCount(const MaterialManager&) const
	{
		return m_PropertyInfos.size();
	}

	MaterialPropertyInfo* MaterialData::GetPropertyInfoAt(const MaterialManager& , size_t index)
	{
		return &m_PropertyInfos[index];
	}

	size_t MaterialData::GetCurrentBufferOffset(const MaterialManager&) const
	{
		return m_CurrentOffset;
	}

	std::vector<char>& MaterialData::GetBufferReference(const MaterialManager&)
	{
		return m_PropertyBuffer;
	}

	std::vector<char>& MaterialData::GetFinalBufferReference(MaterialManager&)
	{
		return m_PropertyBuffer;
	}

	bool MaterialData::GetPropertyInfoIndex(const MaterialManager&, const std::string& name, size_t& index) const
	{
		const uint32_t hash = Reflect::Hash(name.data());
		if (m_HashToPropertyInfoIndex.find(hash) == m_HashToPropertyInfoIndex.end()) return false;
		index = m_HashToPropertyInfoIndex.at(hash);
		return true;
	}

	size_t MaterialData::ResourceCount() const
	{
		return m_Resources.size();
	}

	AssetReference<TextureData>* MaterialData::GetResourceUUIDPointer(MaterialManager&, size_t index)
	{
		return &m_Resources[index];
	}

	size_t MaterialData::GetResourcePropertyCount(MaterialManager&) const
	{
		return m_ResourcePropertyInfoIndices.size();
	}

	MaterialPropertyInfo* MaterialData::GetResourcePropertyInfo(MaterialManager&, size_t index)
	{
		size_t propertyIndex = m_ResourcePropertyInfoIndices[index];
		return &m_PropertyInfos[index];
	}

	size_t MaterialData::GetPropertyIndexFromResourceIndex(MaterialManager&, size_t index) const
	{
		return m_ResourcePropertyInfoIndices[index];
	}

	void MaterialData::ClearProperties()
	{
		m_PropertyBuffer.clear();
		m_PropertyInfos.clear();
		m_ResourcePropertyInfoIndices.clear();
		m_Resources.clear();
		m_HashToPropertyInfoIndex.clear();
		m_CurrentOffset = 0;
	}

	void MaterialData::Serialize(BinaryStream& container) const
	{
		/* Write pipeline ID */
		container.Write(m_Pipeline);

		/* Write property infos */
		container.Write(m_PropertyInfos.size());
		for (size_t i = 0; i < m_PropertyInfos.size(); ++i)
		{
			const MaterialPropertyInfo& prop = m_PropertyInfos[i];
			container.Write(prop.TypeHash());
			container.Write(prop.ShaderName());
			container.Write(prop.DisplayName());
			container.Write(prop.Size());
			container.Write(prop.Offset());
			container.Write(prop.IsResource());
			container.Write(prop.Flags());
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

	void MaterialData::Deserialize(BinaryStream& container)
	{
		/* Read pipeline ID */
		container.Read(m_Pipeline);

		/* Read property infos */
		size_t numProperties;
		container.Read(numProperties);
		m_PropertyInfos.resize(numProperties);
		for (size_t i = 0; i < m_PropertyInfos.size(); ++i)
		{
			MaterialPropertyInfo& prop = m_PropertyInfos[i];
			container.Read(prop.m_TypeHash);
			container.Read(prop.m_PropertyShaderName);
			container.Read(prop.m_PropertyDisplayName);
			container.Read(prop.m_Size);
			container.Read(prop.m_Offset);
			container.Read(prop.m_IsResource);
			container.Read(prop.m_Flags);

			if (!prop.m_IsResource) continue;
			m_ResourcePropertyInfoIndices.push_back(i);
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

	void MaterialData::SetTexture(MaterialManager& materialManager, const std::string& name, TextureData* value)
	{
		const UUID uuid = value ? value->GetUUID() : 0;
		SetTexture(materialManager, name, uuid);
	}

	void MaterialData::SetTexture(MaterialManager& materialManager, const std::string& name, UUID uuid)
	{
		size_t index;
		if (!GetPropertyInfoIndex(materialManager, name, index)) return;
		EnableProperty(index);
		const MaterialPropertyInfo* pPropertyInfo = GetPropertyInfoAt(materialManager, index);
		if (!pPropertyInfo->IsResource()) return;
		const size_t resourceIndex = pPropertyInfo->Offset();
		m_Resources[resourceIndex] = uuid;
	}

	bool MaterialData::GetTexture(MaterialManager& materialManager, const std::string& name, TextureData** value, AssetManager* pManager)
	{
		size_t index;
		if (!GetPropertyInfoIndex(materialManager, name, index)) return false;
		const MaterialPropertyInfo* pPropertyInfo = GetPropertyInfoAt(materialManager, index);
		if (!pPropertyInfo->IsResource()) return false;
		const size_t resourceIndex = pPropertyInfo->Offset();
		*value = m_Resources[resourceIndex].Get(pManager);
		return *value;
	}

	void MaterialData::EnableProperty(size_t)
	{
	}

	std::vector<char>& MaterialData::GetPropertyBuffer(MaterialManager&, size_t)
	{
		return m_PropertyBuffer;
	}
}
