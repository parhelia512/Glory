#include "UIDocumentData.h"
#include "UIComponentManagers.h"

#include <BinaryStream.h>

namespace Glory
{
	Utils::ECS::EntityRegistry& UIEntity::GetRegistry()
	{
		return m_pOwner->m_Registry;
	}

	UIDocumentData::UIDocumentData()
	{
		APPEND_TYPE(UIDocumentData);
	}

	UIDocumentData::~UIDocumentData()
	{
	}

	Utils::ECS::EntityRegistry& UIDocumentData::GetRegistry()
	{
		return m_Registry;
	}

	const std::string& UIDocumentData::Name(Utils::ECS::EntityID entity) const
	{
		return m_Names.at(entity);
	}

	Utils::ECS::EntityID UIDocumentData::CreateEmptyEntity(std::string_view name, UUID uuid)
	{
		Utils::ECS::EntityID entity = m_Registry.CreateEntity();
		m_UUIds.emplace(entity, uuid);
		m_Ids.emplace(uuid, entity);
		m_Names.emplace(entity, name);
		return entity;
	}

	Utils::ECS::EntityID UIDocumentData::CreateEntity(std::string_view name, UUID uuid)
	{
		Utils::ECS::EntityID entity = m_Registry.CreateEntity();
		m_Registry.AddComponent<UITransform>(entity, UUID());
		m_UUIds.emplace(entity, uuid);
		m_Ids.emplace(uuid, entity);
		m_Names.emplace(entity, name);
		return entity;
	}

	UUID UIDocumentData::EntityUUID(Utils::ECS::EntityID entity) const
	{
		return m_UUIds.at(entity);
	}

	Utils::ECS::EntityID UIDocumentData::EntityID(UUID uuid) const
	{
		return m_Ids.at(uuid);
	}

	void UIDocumentData::Reset()
	{
		m_Registry.Reset();
		m_Ids.clear();
		m_UUIds.clear();
		m_Names.clear();
	}

	void UIDocumentData::Serialize(Utils::BinaryStream& container) const
	{
		m_Registry.Serialize(container);

		/* Serialize scene IDs and names */
		container.Write(m_Ids.size());
		for (auto itor = m_Ids.begin(); itor != m_Ids.end(); ++itor)
		{
			container.Write(itor->first).Write(itor->second).Write(m_Names.at(itor->second));
		}
	}

	void UIDocumentData::Deserialize(Utils::BinaryStream& container)
	{
		m_Registry.Deserialize(container);

		/* Deserialize scene IDs and names */
		size_t idSize;
		container.Read(idSize);
		for (size_t i = 0; i < idSize; i++)
		{
			UUID id;
			Utils::ECS::EntityID entity;
			std::string name;
			container.Read(id).Read(entity).Read(name);
			m_Ids.emplace(id, entity);
			m_UUIds.emplace(entity, id);
			m_Names.emplace(entity, name);
		}
	}

	void UIDocumentData::References(IEngine* pEngine, std::vector<UUID>& references) const
	{
		m_Registry.GetReferences(references);
	}
}
