#pragma once
#include "EntityID.h"
#include "IComponentManager.h"
#include "ECSTypeTraits.h"
#include "EntityCallType.h"

#include <Hash.h>
#include <UUID.h>
#include <BitSet.h>

#include <vector>
#include <memory>
#include <unordered_map>
#include <cassert>

namespace Glory::Utils
{
	class BinaryStream;
}

namespace Glory::Utils::ECS
{
	template<ComponentCompatible Component>
	class ComponentManager;

	class EntityRegistry
	{
	public:
		EntityRegistry(void* userData=nullptr, size_t reserveComponentManagers=1, size_t reserveEntities=100);
		virtual ~EntityRegistry();

		EntityID CreateEntity();

		template<IsComponentManager Manager>
		Manager& AddManager()
		{
			const uint32_t hash = Manager::GetComponentHash();
			const uint32_t index = uint32_t(m_ComponentManagers.size());
			auto& newManager = m_ComponentManagers.emplace_back(new Manager());
			m_HashToComponentManagerIndex.emplace(hash, index);
			m_ComponentOrderDirty.Reserve(index + 1ull);
			m_ComponentOrderDirty.Set(index, false);
			newManager->Initialize();
			return static_cast<Manager&>(*newManager);
		}

		void AddManager(IComponentManager* manager);

		template<ComponentCompatible Component>
		Component& AddComponent(EntityID entity, UUID componentID=UUID())
		{
			size_t index = 0;
			ComponentManager<Component>* manager = GetComponentManager<Component>(&index);
			m_ComponentOrderDirty.Set(index);
			m_HasComponent[entity].Set(index);
			m_EntityComponentOrder[entity].emplace_back(manager->ComponentHash(), componentID);
			return *static_cast<Component*>(manager->Add(entity));
		}

		template<ComponentCompatible Component>
		void RemoveComponent(EntityID entity)
		{
			size_t index = 0;
			ComponentManager<Component>* manager = GetComponentManager<Component>(&index);
			assert(m_HasComponent[entity].IsSet(index));
			m_HasComponent[entity].UnSet(index);
			auto iter = std::find_if(m_EntityComponentOrder[entity].begin(), m_EntityComponentOrder[entity].end(),
				[](const std::pair<uint32_t, UUID>& pair) {
					return pair.first == manager->ComponentHash();
				});
			m_EntityComponentOrder[entity].erase(iter);
			return *static_cast<Component*>(manager->Remove(entity));
		}

		template<ComponentCompatible Component>
		Component& GetComponent(EntityID entity)
		{
			ComponentManager<Component>* manager = GetComponentManager<Component>();
			return manager->Get(entity);
		}

		template<ComponentCompatible Component>
		bool HasComponent(EntityID entity)
		{
			size_t index = 0;
			ComponentManager<Component>* manager = GetComponentManager<Component>(&index);
			return m_HasComponent[entity].IsSet(index);
		}

		template<typename Component>
		ComponentManager<Component>* GetComponentManager(size_t* outIndex=nullptr)
		{
			const uint32_t hash = Hashing::Hash(typeid(Component).name());
			return static_cast<ComponentManager<Component>*>(GetComponentManager(hash, outIndex));
		}

		IComponentManager* GetComponentManager(uint32_t componentHash, size_t* outIndex=nullptr);
		IComponentManager* GetComponentManager(uint32_t componentHash, size_t* outIndex=nullptr) const;

		bool EntityValid(EntityID entity) const;
		bool EntityActiveHierarchy(EntityID entity) const;
		bool EntityActiveSelf(EntityID entity) const;

		EntityID GetParent(EntityID entity) const;
		void SetParent(EntityID entity, EntityID parent);

		void SetActive(EntityID entity, bool active);

		void DestroyEntity(EntityID entity);
		void Clear(EntityID entity);

		bool IsEntityDirty(EntityID entity) const;
		void SetEntityDirty(EntityID entity, bool dirty=true);

		size_t ChildCount(EntityID entity) const;
		EntityID Child(EntityID entity, size_t index) const;
		size_t SiblingIndex(EntityID entity) const;
		void SetSiblingIndex(EntityID entity, size_t index);

		size_t EntityComponentCount(EntityID entity) const;
		uint32_t EntityComponentType(EntityID entity, size_t index) const;
		UUID EntityComponentID(EntityID entity, size_t index) const;
		void* GetComponentAddress(EntityID entity, uint32_t type);
		void* GetComponentAddress(EntityID entity, uint32_t type) const;
		void* CopyComponent(EntityID entity, uint32_t type, UUID componentID, void* data);

		template<typename T>
		T* GetUserData()
		{
			return reinterpret_cast<T*>(m_pUserData);
		}

		void Sort();

		void SetUserData(void* data);

		void DisableCalls();

		size_t AliveCount() const;

		void GetReferences(std::vector<UUID>& references) const;

		void Serialize(BinaryStream& container) const;
		void Deserialize(BinaryStream& container);

		bool operator==(const EntityRegistry& other) const;

	public: /* Global calls */
		void Validate();
		void Activate();
		void Deactivate();
		void EnableDraw();
		void DisableDraw();
		void Start();
		void Stop();
		void Update(float dt);
		void Draw();

	public: /* Individual calls */
		void CallOnValidate(EntityID entity);
		void CallStart(EntityID entity);
		void CallOnActivate(EntityID entity);

	public:
		void EnableCall(EntityCallType callType, bool enable=true);
		bool IsCallEnabled(EntityCallType callType) const;

	private:
		void SetHierarchyActiveStateChildren(EntityID entity, bool active);

	private:
		std::vector<std::unique_ptr<IComponentManager>> m_ComponentManagers;
		std::unordered_map<uint32_t, uint32_t> m_HashToComponentManagerIndex;
		BitSet m_ComponentOrderDirty;

		BitSet m_EntityAlive;
		BitSet m_EntityActiveSelf;
		BitSet m_EntityActiveHierarchy;
		BitSet m_EntityDirty;

		std::vector<std::vector<EntityID>> m_EntityTrees;
		std::vector<EntityID> m_Parents;
		std::vector<BitSet> m_HasComponent;
		std::vector<std::vector<std::pair<uint32_t, UUID>>> m_EntityComponentOrder;

		EntityID m_NextEntityID;
		size_t m_AliveCount;

		BitSet m_EnabledCalls;

		void* m_pUserData;

		bool m_CallsEnabled = true;

		/* TODO */
		/*
		 * - Keep track of component order per entity
		 */
	};
}
