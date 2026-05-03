#include "Resources.h"
#include "Debug.h"
#include "ResourceReferencing.h"
#include "AssetDatabase.h"

#include <Hash.h>
#include <GloryAssert.h>

namespace Glory
{
	Resources::Resources(AssetDatabase* pDatabase, ResourceTypes* pResourceTypes, Debug* pDebug):
		m_pDatabase(pDatabase), m_pResourceTypes(pResourceTypes), m_pDebug(pDebug)
	{
		SetResourcesInstance(this);
	}

	bool Resources::AddResource(Resource** pResource)
	{
		std::type_index type = typeid(Object);
		for (size_t i = 0; i < (*pResource)->TypeCount(); ++i)
		{
			(*pResource)->GetType(i, type);
			const uint32_t typeHash = Hashing::Hash(type.name());
			auto iter = m_HashToManagerIndex.find(typeHash);
			if (iter == m_HashToManagerIndex.end()) continue;
			Resource* pAddedResource = m_Managers.at(iter->second)->Add((*pResource));
			m_ResourceIDToManagerIndex.emplace(pAddedResource->GetUUID(), iter->second);
			*pResource = pAddedResource;
			return true;
		}

		(*pResource)->GetType(0, type);
		m_pDebug->LogError(std::format("Resources::AddResource: Could not add resource, missing resource manager for type {}", type.name()));
		return false;
	}

	Resource* Resources::GetResource(UUID id)
	{
		auto iter = m_ResourceIDToManagerIndex.find(id);
		if (iter == m_ResourceIDToManagerIndex.end()) return nullptr;
		return m_Managers.at(iter->second)->Get(id);
	}

	ResourceTypes* Resources::GetResourceTypes()
	{
		return m_pResourceTypes;
	}

	void Resources::AddReference(UUID id)
	{
		if (!m_ReferenceCountingAllowed) return;
		size_t& counter = m_ReferenceCounter[id];
		++counter;
		if (counter != 1) return;
		if (!m_pDatabase->AssetExists(id)) return;
		/* @todo: Check if current thread id is not the default thread and queue immedeitaly if it is not */

		m_ToUnloadResources.erase(id);
		m_ToLoadResources.insert(id);
	}

	void Resources::RemoveReference(UUID id)
	{
		if (!m_ReferenceCountingAllowed) return;
		size_t& counter = m_ReferenceCounter[id];
		GLORY_ASSERT(counter > 0, "Incorrect reference count!");
		--counter;
		if (counter != 0) return;
		if (!m_pDatabase->AssetExists(id)) return;
		m_ToUnloadResources.insert(id);
	}

	void Resources::HandleToLoad(std::function<void(UUID)> callback)
	{
		for (auto iter = m_ToLoadResources.begin(); iter != m_ToLoadResources.end(); ++iter)
			callback(*iter);
		m_ToLoadResources.clear();
	}

	void Resources::HandleToUnload(std::function<void(UUID)> callback)
	{
		for (auto iter = m_ToUnloadResources.begin(); iter != m_ToUnloadResources.end(); ++iter)
			callback(*iter);
		m_ToUnloadResources.clear();
	}

	void Resources::UnloadResource(UUID id)
	{
		auto iter = m_ResourceIDToManagerIndex.find(id);
		if (iter == m_ResourceIDToManagerIndex.end()) return;
		m_Managers.at(iter->second)->Get(id);
	}

	void Resources::SetAllowReferenceCounting(bool allowed)
	{
		m_ReferenceCountingAllowed = allowed;
	}
}
