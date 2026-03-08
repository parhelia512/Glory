#pragma once
#include "EntityID.h"
#include "IComponentManager.h"
#include "ECSTypeTraits.h"
#include "EntityCallType.h"

#include <SparseSet.h>
#include <Hash.h>
#include <BitSet.h>

namespace Glory::Utils::ECS
{
	class EntityRegistry;

	template<ComponentCompatible Component>
	class ComponentManager : public SparseSet<EntityID, Component>, public IComponentManager
	{
	public:
		ComponentManager(EntityRegistry* pRegistry, size_t capacity = 32) :
			m_pRegistry(pRegistry), SparseSet<EntityID, Component>{ 1000, capacity },
			m_ComponentActive(capacity), m_ActiveSize(0) {
		}
		virtual ~ComponentManager() = default;

		static uint32_t GetComponentHash()
		{
			return Hashing::Hash(typeid(Component).name());
		}

		virtual void Initialize()
		{
			OnInitialize();
		}

		virtual uint32_t ComponentHash() const override
		{
			return ComponentTypeHash;
		}

		virtual void* Add(EntityID entity) override
		{
			return &SparseSet<EntityID, Component>::Add(entity, Component());
		}

		virtual void Remove(EntityID entity) override
		{
			SparseSet<EntityID, Component>::Remove(entity);
		}

		Component& Get(EntityID entity)
		{
			return SparseSet<EntityID, Component>::Get(entity);
		}

		void Sort(const std::vector<std::vector<EntityID>>& entityTrees) override
		{
			size_t currentIndex = 0;
			SortRecursive(entityTrees, currentIndex);
			m_ActiveSize = currentIndex;
		}

		virtual bool IsActive(EntityID entity) override
		{
			const size_t index = SparseSet<EntityID, Component>::Index(entity);
			if (index != SparseSet<EntityID, Component>::InvalidIndex) return false;
			return m_ComponentActive.IsSet(index);
		}

		virtual void Activate(EntityID entity) override
		{
			const size_t index = SparseSet<EntityID, Component>::Index(entity);
			if (index != SparseSet<EntityID, Component>::InvalidIndex) return;
			const bool wasActive = m_ComponentActive.IsSet(index);
			if (wasActive) return;
			m_ComponentActive.Set(index);
			const bool entityActive = m_pRegistry->EntityActiveHierarchy(entity);
			if (!entityActive) return;
			CallOnActivate(entity);
			CallOnEnableDraw(entity);
		}

		virtual void Deactivate(EntityID entity) override
		{
			const size_t index = SparseSet<EntityID, Component>::Index(entity);
			if (index != SparseSet<EntityID, Component>::InvalidIndex) return;
			const bool wasActive = m_ComponentActive.IsSet(index);
			if (!wasActive) return;
			m_ComponentActive.UnSet(index);
			const bool entityActive = m_pRegistry->EntityActiveHierarchy(entity);
			if (!entityActive) return;
			CallOnDeactivate(entity);
			CallOnDisableDraw(entity);
		}

	protected: /* Custom implementations, these are always called */
		virtual void OnInitialize() {}
		virtual void OnAddComponent(EntityID, Component&) {}
		virtual void OnRemoveComponent(EntityID, size_t) {}
		virtual void OnReserveComponents() {}
		virtual void OnReserveIDs() {}
		virtual void OnSwapComponents(size_t index1, size_t index2) {}

	protected: /* Component callbacks */
		typedef void (ComponentManager<Component>::*Function)(EntityID, Component&);
		typedef void (ComponentManager<Component>::*UpdateFunction)(EntityID, Component&, float);
		Function DoValidate = nullptr;
		Function DoOnAdd = nullptr;
		Function DoOnRemove = nullptr;
		Function DoOnActivate = nullptr;
		Function DoOnDeactivate = nullptr;
		Function DoOnEnableDraw = nullptr;
		Function DoOnDisableDraw = nullptr;
		Function DoStart = nullptr;
		Function DoStop = nullptr;
		UpdateFunction DoPreUpdate = nullptr;
		UpdateFunction DoUpdate = nullptr;
		UpdateFunction DoPostUpdate = nullptr;
		Function DoPreDraw = nullptr;
		Function DoDraw = nullptr;
		Function DoPostDraw = nullptr;

		template<typename Manager>
		void Bind(Function& target, void(Manager::*func)(EntityID, Component&))
		{
			target = static_cast<Function>(func);
		}

		template<typename Manager>
		void Bind(UpdateFunction& target, void(Manager::* func)(EntityID, Component&, float))
		{
			target = static_cast<UpdateFunction>(func);
		}

	private:
		virtual void OnAdd(size_t denseIndex, EntityID entity, Component& component) override final
		{
			m_ComponentActive.Set(denseIndex);
			++m_ActiveSize;
			OnAddComponent(entity, component);
			CallOnAdd(entity);
		}

		virtual void OnRemove(EntityID entity, size_t index) override final
		{
			m_ComponentActive.Set(index, false);
			--m_ActiveSize;
			OnRemoveComponent(entity, index);
			CallOnRemove(entity);
		}

		virtual void OnReserveDense() override final
		{
			m_ComponentActive.Reserve(SparseSet<EntityID, Component>::DenseCapacity());
			OnReserveComponents();
		}

		virtual void OnReserveSparse() override final
		{
			OnReserveIDs();
		}

		virtual void OnSwap(size_t index1, size_t index2) override final
		{
			const bool enabled1 = m_ComponentActive.IsSet(index1);
			const bool enabled2 = m_ComponentActive.IsSet(index2);
			m_ComponentActive.Set(index1, enabled2);
			m_ComponentActive.Set(index2, enabled1);
			OnSwapComponents(index1, index2);
		}

	private: /* Global component callbacks */
		virtual void Start() override final
		{
			if (!DoStart) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::Start)) return;

			const size_t numComponents = SparseSet<EntityID, Component>::Size();
			for (size_t i = 0; i < numComponents; ++i)
			{
				const EntityID entity = SparseSet<EntityID, Component>::DenseID(i);
				(this->*DoStart)(entity, SparseSet<EntityID, Component>::GetAt(i));
			}
		}

		virtual void Stop() override final
		{
			if (!DoStop) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::Stop)) return;

			const size_t numComponents = SparseSet<EntityID, Component>::Size();
			for (size_t i = 0; i < numComponents; ++i)
			{
				const EntityID entity = SparseSet<EntityID, Component>::DenseID(i);
				(this->*DoStop)(entity, SparseSet<EntityID, Component>::GetAt(i));
			}
		}

		virtual void PreUpdate(float dt) override final
		{
			if (!DoPreUpdate) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::PreUpdate)) return;

			for (size_t i = 0; i < m_ActiveSize; ++i)
			{
				const EntityID entity = SparseSet<EntityID, Component>::DenseID(i);
				(this->*DoPreUpdate)(entity, SparseSet<EntityID, Component>::GetAt(i), dt);
			}
		}

		virtual void Update(float dt) override final
		{
			if (!DoUpdate) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::Update)) return;

			for (size_t i = 0; i < m_ActiveSize; ++i)
			{
				const EntityID entity = SparseSet<EntityID, Component>::DenseID(i);
				(this->*DoUpdate)(entity, SparseSet<EntityID, Component>::GetAt(i), dt);
			}
		}

		virtual void PostUpdate(float dt) override final
		{
			if (!DoPostUpdate) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::PostUpdate)) return;

			for (size_t i = 0; i < m_ActiveSize; ++i)
			{
				const EntityID entity = SparseSet<EntityID, Component>::DenseID(i);
				(this->*DoPostUpdate)(entity, SparseSet<EntityID, Component>::GetAt(i), dt);
			}
		}

		virtual void PreDraw() override final
		{
			if (!DoPreDraw) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::PreDraw)) return;

			for (size_t i = 0; i < m_ActiveSize; ++i)
			{
				const EntityID entity = SparseSet<EntityID, Component>::DenseID(i);
				(this->*DoPreDraw)(entity, SparseSet<EntityID, Component>::GetAt(i));
			}
		}

		virtual void Draw() override final
		{
			if (!DoDraw) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::Draw)) return;

			for (size_t i = 0; i < m_ActiveSize; ++i)
			{
				const EntityID entity = SparseSet<EntityID, Component>::DenseID(i);
				(this->*DoDraw)(entity, SparseSet<EntityID, Component>::GetAt(i));
			}
		}

		virtual void PostDraw() override final
		{
			if (!DoPostDraw) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::PostDraw)) return;

			for (size_t i = 0; i < m_ActiveSize; ++i)
			{
				const EntityID entity = SparseSet<EntityID, Component>::DenseID(i);
				(this->*DoPostDraw)(entity, SparseSet<EntityID, Component>::GetAt(i));
			}
		}

		void SortRecursive(const std::vector<std::vector<EntityID>>& entityTrees, size_t& currentIndex, EntityID root=0ull)
		{
			for (size_t i = 0; i < entityTrees[root].size(); ++i)
			{
				const EntityID child = entityTrees[root][i];
				const size_t index = SparseSet<EntityID, Component>::Index(child);
				if (index == SparseSet<EntityID, Component>::InvalidIndex) continue;
				if (!m_ComponentActive.IsSet(i) || !m_pRegistry->EntityActiveHierarchy(child)) continue;
				SparseSet<EntityID, Component>::Swap(currentIndex, index);
				++currentIndex;
				SortRecursive(entityTrees, currentIndex, child);
			}
		}

	private: /* Manual calls */
		virtual void CallValidate(EntityID entity) override
		{
			if (!DoValidate) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::OnValidate)) return;
			(this->*DoValidate)(entity, SparseSet<EntityID, Component>::Get(entity));
		}

		virtual void CallOnAdd(EntityID entity) override
		{
			if (!DoOnAdd) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::OnAdd)) return;
			(this->*DoOnAdd)(entity, SparseSet<EntityID, Component>::Get(entity));
		}

		virtual void CallOnRemove(EntityID entity) override
		{
			if (!DoOnRemove) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::OnRemove)) return;
			(this->*DoOnRemove)(entity, SparseSet<EntityID, Component>::Get(entity));
		}

		virtual void CallOnActivate(EntityID entity) override
		{
			if (!DoOnActivate) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::OnActivate)) return;
			(this->*DoOnActivate)(entity, SparseSet<EntityID, Component>::Get(entity));
		}

		virtual void CallOnDeactivate(EntityID entity) override
		{
			if (!DoOnDeactivate) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::OnDeactivate)) return;
			(this->*DoOnDeactivate)(entity, SparseSet<EntityID, Component>::Get(entity));
		}

		virtual void CallOnEnableDraw(EntityID entity) override
		{
			if (!DoOnEnableDraw) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::OnEnableDraw)) return;
			(this->*DoOnEnableDraw)(entity, SparseSet<EntityID, Component>::Get(entity));
		}

		virtual void CallOnDisableDraw(EntityID entity) override
		{
			if (!DoOnDisableDraw) return;
			if (!m_pRegistry->IsCallEnabled(EntityCallType::OnDisableDraw)) return;
			(this->*DoOnDisableDraw)(entity, SparseSet<EntityID, Component>::Get(entity));
		}

	protected:
		const uint32_t ComponentTypeHash = Hashing::Hash(typeid(Component).name());
		EntityRegistry* m_pRegistry;
		BitSet m_ComponentActive;
		size_t m_ActiveSize;
	};
}
