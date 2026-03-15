#include <Tester.h>
#include <FileComparers.h>

#include <ComponentManager.h>
#include <EntityRegistry.h>
#include <RegistryFactory.h>

#include <array>

template<>
struct std::formatter<Glory::UUID, char>
{
	bool quoted = false;

	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		return std::formatter<uint64_t, char>().parse(ctx);
	}

	template<class FmtContext>
	FmtContext::iterator format(Glory::UUID id, FmtContext& ctx) const
	{
		return std::formatter<uint64_t, char>().format(uint64_t(id), ctx);
	}
};

namespace Glory::Test
{
	struct Transform
	{
	public:
		Transform() : X(0.0f), Y(0.0f) {};

		float X, Y;
	};

	struct Velocity
	{
	public:
		Velocity() : X(0.0f), Y(0.0f) {};
		Velocity(float x, float y) : X(x), Y(y) {};

		float X, Y;
	};

	struct CallsCollector
	{
	public:
		size_t m_OnAddCalls = 0;
		size_t m_OnRemoveCalls = 0;
		size_t m_OnValidateCalls = 0;
		size_t m_OnActivateCalls = 0;
		size_t m_OnDeactivateCalls = 0;
		size_t m_OnEnableDrawCalls = 0;
		size_t m_OnDisableDrawCalls = 0;
		size_t m_OnDirtyCalls = 0;
		size_t m_StartCalls = 0;
		size_t m_StopCalls = 0;
		size_t m_PreUpdateCalls = 0;
		size_t m_UpdateCalls = 0;
		size_t m_PostUpdateCalls = 0;
		size_t m_PreDrawCalls = 0;
		size_t m_DrawCalls = 0;
		size_t m_PostDrawCalls = 0;
	};

	class ECSTest : public Utils::Tester
	{
	public:
		ECSTest();
		virtual ~ECSTest();

	private:
		void Initialize();
		void Cleanup();

		void Factory();
		void CreateEntities();
		void AddComponents();
		void RemoveComponents();
		void UpdateComponents();
		void ComponentStates();
		void EntityStates();
		void ComponentCalls();
		void Hierarchy();
		void Serialize();
		void SerializeAndDeserialize();
		void DestroyEntity();

	private:
		Utils::ECS::RegistryFactory m_RegistryFactory;
	};

	class TransformManager : public Utils::ECS::ComponentManager<Transform>
	{
	public:
		TransformManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=100) :
			ComponentManager(pRegistry, capacity), m_pTest(nullptr) {
		};
		virtual ~TransformManager() = default;

		void DoComponentAdd(Utils::ECS::EntityID entity, Transform& velocity)
		{
			++m_ComponentAddCalls;
		}

		void DoComponentRemove(Utils::ECS::EntityID entity, Transform& velocity)
		{
			++m_ComponentRemovedCalls;
		}

		virtual void OnInitialize() override
		{
			Bind(DoOnAdd, &TransformManager::DoComponentAdd);
			Bind(DoOnRemove, &TransformManager::DoComponentRemove);
		}

		ECSTest* m_pTest;
		size_t m_ComponentAddCalls = 0;
		size_t m_ComponentRemovedCalls = 0;
	};

	class VelocityManager : public Utils::ECS::ComponentManager<Velocity>
	{
	public:
		VelocityManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=100) :
			ComponentManager(pRegistry, capacity), m_pTest(nullptr) {
		};
		virtual ~VelocityManager() = default;

		virtual void OnInitialize() override
		{
			Bind(DoUpdate, &VelocityManager::DoUpdateImpl);
			Bind(DoOnAdd, &VelocityManager::DoComponentAdd);
			Bind(DoOnRemove, &VelocityManager::DoComponentRemove);
		}

		void DoComponentAdd(Utils::ECS::EntityID entity, Velocity& velocity)
		{
			++m_ComponentAddCalls;
		}

		void DoComponentRemove(Utils::ECS::EntityID entity, Velocity& velocity)
		{
			++m_ComponentRemovedCalls;
		}

		void DoUpdateImpl(Utils::ECS::EntityID entity, Velocity& velocity, float dt)
		{
			Transform& transform = m_pRegistry->GetComponent<Transform>(entity);
			transform.X += dt*velocity.X;
			transform.Y += dt*velocity.Y;
		}

		ECSTest* m_pTest;
		size_t m_ComponentAddCalls = 0;
		size_t m_ComponentRemovedCalls = 0;
	};

	class CallsCollectorManager : public Utils::ECS::ComponentManager<CallsCollector>
	{
	public:
		CallsCollectorManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=100) :
			ComponentManager(pRegistry, capacity) {
		};
		virtual ~CallsCollectorManager() = default;

		virtual void OnInitialize() override
		{
			Bind(DoValidate, &CallsCollectorManager::DoValidateImpl);
			Bind(DoStart, &CallsCollectorManager::DoStartImpl);
			Bind(DoStop, &CallsCollectorManager::DoStopImpl);
			Bind(DoOnActivate, &CallsCollectorManager::DoActivateImpl);
			Bind(DoOnDeactivate, &CallsCollectorManager::DoDeactivateImpl);
			Bind(DoOnEnableDraw, &CallsCollectorManager::DoEnableDrawImpl);
			Bind(DoOnDisableDraw, &CallsCollectorManager::DoDisableDrawImpl);
			Bind(DoOnAdd, &CallsCollectorManager::DoComponentAddImpl);
			Bind(DoOnRemove, &CallsCollectorManager::DoComponentRemoveImpl);
			Bind(DoOnDirty, &CallsCollectorManager::DoOnDirtyImpl);
			Bind(DoPreUpdate, &CallsCollectorManager::DoPreUpdateImpl);
			Bind(DoUpdate, &CallsCollectorManager::DoUpdateImpl);
			Bind(DoPostUpdate, &CallsCollectorManager::DoPostUpdateImpl);
			Bind(DoPreDraw, &CallsCollectorManager::DoPreDrawImpl);
			Bind(DoDraw, &CallsCollectorManager::DoDrawImpl);
			Bind(DoPostDraw, &CallsCollectorManager::DoPostDrawImpl);
		}

		void DoValidateImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_OnValidateCalls;
		}

		void DoStartImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_StartCalls;
		}

		void DoStopImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_StopCalls;
		}

		void DoActivateImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_OnActivateCalls;
		}

		void DoDeactivateImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_OnDeactivateCalls;
		}

		void DoEnableDrawImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_OnEnableDrawCalls;
		}

		void DoDisableDrawImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_OnDisableDrawCalls;
		}

		void DoComponentAddImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_OnAddCalls;
		}

		void DoComponentRemoveImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_OnRemoveCalls;
		}

		void DoOnDirtyImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_OnDirtyCalls;
		}

		void DoPreUpdateImpl(Utils::ECS::EntityID entity, CallsCollector& collector, float dt)
		{
			++collector.m_PreUpdateCalls;
		}

		void DoUpdateImpl(Utils::ECS::EntityID entity, CallsCollector& collector, float dt)
		{
			++collector.m_UpdateCalls;
		}

		void DoPostUpdateImpl(Utils::ECS::EntityID entity, CallsCollector& collector, float dt)
		{
			++collector.m_PostUpdateCalls;
		}

		void DoPreDrawImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_PreDrawCalls;
		}

		void DoDrawImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_DrawCalls;
		}

		void DoPostDrawImpl(Utils::ECS::EntityID entity, CallsCollector& collector)
		{
			++collector.m_PostDrawCalls;
		}
	};

	ECSTest::ECSTest()
	{
		AddTests({ &ECSTest::Factory,
				&ECSTest::CreateEntities,
				&ECSTest::AddComponents,
				&ECSTest::RemoveComponents,
				&ECSTest::UpdateComponents,
				&ECSTest::ComponentStates,
				&ECSTest::EntityStates,
				&ECSTest::ComponentCalls,
				&ECSTest::Hierarchy,
				&ECSTest::Serialize,
				&ECSTest::SerializeAndDeserialize,
				&ECSTest::DestroyEntity,
			},
			&ECSTest::Initialize, &ECSTest::Cleanup);
	}

	ECSTest::~ECSTest()
	{
	}

	void ECSTest::Initialize()
	{
		m_RegistryFactory.RegisterComponentManager<TransformManager>(
			[this](Utils::ECS::EntityRegistry*, TransformManager* pManager) {
				pManager->m_pTest = this;
			});
		m_RegistryFactory.RegisterComponentManager<VelocityManager>(
			[this](Utils::ECS::EntityRegistry*, VelocityManager* pManager) {
				pManager->m_pTest = this;
			});
		m_RegistryFactory.RegisterComponentManager<CallsCollectorManager>();
	}

	void ECSTest::Cleanup()
	{
		m_RegistryFactory.Clear();
	}

	void ECSTest::Factory()
	{
		Utils::ECS::EntityRegistry registry;
		GLORY_TEST_COMPARE(registry.ComponentManagerCount(), 0ull);
		GLORY_TEST_COMPARE(registry.MaxEntityID(), 1ull);
		GLORY_TEST_COMPARE(registry.AliveCount(), 0ull);

		m_RegistryFactory.PopulateRegisry(registry);
		GLORY_TEST_COMPARE(registry.ComponentManagerCount(), 3ull);
		GLORY_TEST_COMPARE(registry.MaxEntityID(), 1ull);
		GLORY_TEST_COMPARE(registry.AliveCount(), 0ull);

		auto transforms = registry.GetComponentManager<Transform>();
		auto velocities = registry.GetComponentManager<Velocity>();
		GLORY_TEST_VERIFY(transforms);
		GLORY_TEST_VERIFY(velocities);

		if (transforms)
			GLORY_TEST_VERIFY(static_cast<TransformManager*>(transforms)->m_pTest);
		if (velocities)
			GLORY_TEST_VERIFY(static_cast<VelocityManager*>(velocities)->m_pTest);
	}

	void ECSTest::CreateEntities()
	{
		Utils::ECS::EntityRegistry registry;

		constexpr const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount; ++i)
		{
			Utils::ECS::EntityID entity = registry.CreateEntity();
			GLORY_TEST_COMPARE(entity, i + 1);
		}

		GLORY_TEST_COMPARE(registry.ComponentManagerCount(), 0ull);
		GLORY_TEST_COMPARE(registry.MaxEntityID(), entityCount + 1);
		GLORY_TEST_COMPARE(registry.AliveCount(), entityCount);

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			GLORY_TEST_COMPARE(registry.EntityValid(entity), true);
			GLORY_TEST_COMPARE(registry.EntityComponentCount(entity), 0ull);
			GLORY_TEST_COMPARE(registry.EntityActiveSelf(entity), true);
			GLORY_TEST_COMPARE(registry.EntityActiveHierarchy(entity), true);
			GLORY_TEST_COMPARE(registry.ChildCount(entity), 0ull);
			GLORY_TEST_COMPARE(registry.IsEntityDirty(entity), true);
			GLORY_TEST_COMPARE(registry.GetParent(entity), 0ull);
		}
	}

	void ECSTest::AddComponents()
	{
		Utils::ECS::EntityRegistry registry;
		m_RegistryFactory.PopulateRegisry(registry);

		auto transforms = registry.GetComponentManager<Transform>();
		auto velocities = registry.GetComponentManager<Velocity>();

		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentAddCalls, 0ull);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentAddCalls, 0ull);
		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentRemovedCalls, 0ull);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentRemovedCalls, 0ull);

		std::vector<UUID> transformIDs;
		std::vector<UUID> velocityIDs;

		constexpr const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount; ++i)
		{
			Utils::ECS::EntityID entity = registry.CreateEntity();
			GLORY_TEST_COMPARE(entity, i + 1);
			GLORY_TEST_FAIL(registry.HasComponent<Transform>(entity));
			GLORY_TEST_FAIL(registry.HasComponent<Velocity>(entity));
			GLORY_TEST_COMPARE(registry.EntityComponentCount(entity), 0ull);
			transformIDs.emplace_back();
			registry.AddComponent<Transform>(entity, transformIDs.back());
			GLORY_TEST_VERIFY(registry.HasComponent<Transform>(entity));
			GLORY_TEST_FAIL(registry.HasComponent<Velocity>(entity));
			GLORY_TEST_COMPARE(registry.EntityComponentCount(entity), 1ull);
			velocityIDs.emplace_back();
			registry.AddComponent<Velocity>(entity, velocityIDs.back());
			GLORY_TEST_VERIFY(registry.HasComponent<Transform>(entity));
			GLORY_TEST_VERIFY(registry.HasComponent<Velocity>(entity));
			GLORY_TEST_COMPARE(registry.EntityComponentCount(entity), 2ull);
		}

		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentAddCalls, entityCount);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentAddCalls, entityCount);
		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentRemovedCalls, 0ull);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentRemovedCalls, 0ull);

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			GLORY_TEST_COMPARE(registry.EntityComponentID(entity, 0), transformIDs[i]);
			GLORY_TEST_COMPARE(registry.EntityComponentType(entity, 0), transforms->GetComponentHash());
			GLORY_TEST_COMPARE(registry.EntityComponentID(entity, 1), velocityIDs[i]);
			GLORY_TEST_COMPARE(registry.EntityComponentType(entity, 1), velocities->GetComponentHash());
		}
	}

	void ECSTest::RemoveComponents()
	{
		Utils::ECS::EntityRegistry registry;
		m_RegistryFactory.PopulateRegisry(registry);

		auto transforms = registry.GetComponentManager<Transform>();
		auto velocities = registry.GetComponentManager<Velocity>();

		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentAddCalls, 0ull);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentAddCalls, 0ull);
		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentRemovedCalls, 0ull);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentRemovedCalls, 0ull);

		constexpr const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount; ++i)
		{
			Utils::ECS::EntityID entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, UUID());
			registry.AddComponent<Velocity>(entity, UUID());
		}

		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentAddCalls, entityCount);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentAddCalls, entityCount);
		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentRemovedCalls, 0ull);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentRemovedCalls, 0ull);

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			GLORY_TEST_VERIFY(registry.HasComponent<Transform>(entity));
			GLORY_TEST_VERIFY(registry.HasComponent<Velocity>(entity));
			GLORY_TEST_COMPARE(registry.EntityComponentCount(entity), 2ull);
			registry.RemoveComponent<Transform>(entity);
			GLORY_TEST_FAIL(registry.HasComponent<Transform>(entity));
			GLORY_TEST_VERIFY(registry.HasComponent<Velocity>(entity));
			GLORY_TEST_COMPARE(registry.EntityComponentCount(entity), 1ull);
			registry.RemoveComponent<Velocity>(entity);
			GLORY_TEST_FAIL(registry.HasComponent<Transform>(entity));
			GLORY_TEST_FAIL(registry.HasComponent<Velocity>(entity));
			GLORY_TEST_COMPARE(registry.EntityComponentCount(entity), 0ull);
		}

		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentAddCalls, entityCount);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentAddCalls, entityCount);
		GLORY_TEST_COMPARE(static_cast<TransformManager*>(transforms)->m_ComponentRemovedCalls, entityCount);
		GLORY_TEST_COMPARE(static_cast<VelocityManager*>(velocities)->m_ComponentRemovedCalls, entityCount);
	}

	void ECSTest::UpdateComponents()
	{
		Utils::ECS::EntityRegistry registry;
		m_RegistryFactory.PopulateRegisry(registry);
		constexpr const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount; ++i)
		{
			Utils::ECS::EntityID entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, UUID());
			registry.AddComponent<Velocity>(entity, UUID(), 1.0f, 1.0f);
		}

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			Transform& transform = registry.GetComponent<Transform>(entity);
			GLORY_TEST_COMPARE(transform.X, 0.0f);
			GLORY_TEST_COMPARE(transform.Y, 0.0f);
		}

		registry.Update(1.0f);

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			Transform& transform = registry.GetComponent<Transform>(entity);
			GLORY_TEST_COMPARE(transform.X, 1.0f);
			GLORY_TEST_COMPARE(transform.Y, 1.0f);
		}

		registry.Update(1.0f);

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			Transform& transform = registry.GetComponent<Transform>(entity);
			GLORY_TEST_COMPARE(transform.X, 2.0f);
			GLORY_TEST_COMPARE(transform.Y, 2.0f);
		}
	}

	void ECSTest::ComponentStates()
	{
		Utils::ECS::EntityRegistry registry;
		m_RegistryFactory.PopulateRegisry(registry);
		constexpr const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount; ++i)
		{
			Utils::ECS::EntityID entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, UUID());
			registry.AddComponent<Velocity>(entity, UUID(), 1.0f, 1.0f);
		}

		auto transforms = registry.GetComponentManager<Transform>();
		auto velocities = registry.GetComponentManager<Velocity>();

		GLORY_TEST_COMPARE(transforms->ActiveSize(), entityCount);
		GLORY_TEST_COMPARE(velocities->ActiveSize(), entityCount);

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			GLORY_TEST_VERIFY(transforms->IsActive(entity));
			GLORY_TEST_VERIFY(velocities->IsActive(entity));

			/* Activating does nothing if already active */
			transforms->Activate(entity);
			velocities->Activate(entity);
			GLORY_TEST_VERIFY(transforms->IsActive(entity));
			GLORY_TEST_VERIFY(velocities->IsActive(entity));

			transforms->Deactivate(entity);
			velocities->Deactivate(entity);
			GLORY_TEST_FAIL(transforms->IsActive(entity));
			GLORY_TEST_FAIL(velocities->IsActive(entity));

			/* Active component counts will not update until we sort */
			GLORY_TEST_COMPARE(transforms->ActiveSize(), entityCount);
			GLORY_TEST_COMPARE(velocities->ActiveSize(), entityCount);
		}

		registry.Sort();
		GLORY_TEST_COMPARE(transforms->ActiveSize(), 0ull);
		GLORY_TEST_COMPARE(velocities->ActiveSize(), 0ull);
	}

	void ECSTest::EntityStates()
	{
		Utils::ECS::EntityRegistry registry;
		m_RegistryFactory.PopulateRegisry(registry);
		constexpr const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount; ++i)
		{
			Utils::ECS::EntityID entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, UUID());
		}

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			GLORY_TEST_VERIFY(registry.EntityActiveSelf(entity));

			/* Activating already active does nothing */
			registry.SetActive(entity, true);
			GLORY_TEST_VERIFY(registry.EntityActiveSelf(entity));
			GLORY_TEST_VERIFY(registry.EntityActiveHierarchy(entity));

			registry.SetActive(entity, false);
			GLORY_TEST_FAIL(registry.EntityActiveSelf(entity));
			GLORY_TEST_FAIL(registry.EntityActiveHierarchy(entity));
		}

		/* Disabled entities are still alive */
		GLORY_TEST_COMPARE(registry.AliveCount(), entityCount);

		/* Active transforms count will not update until the next sort */
		auto transforms = registry.GetComponentManager<Transform>();
		GLORY_TEST_COMPARE(transforms->ActiveSize(), entityCount);

		registry.Sort();
		GLORY_TEST_COMPARE(transforms->ActiveSize(), 0ull);
	}

	void ECSTest::ComponentCalls()
	{
		Utils::ECS::EntityRegistry registry;
		m_RegistryFactory.PopulateRegisry(registry);
		constexpr const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount; ++i)
		{
			Utils::ECS::EntityID entity = registry.CreateEntity();
			registry.AddComponent<CallsCollector>(entity, UUID());
		}

		auto callsCollectors = registry.GetComponentManager<CallsCollector>();
		for (size_t i = 0; i < entityCount; ++i)
		{
			CallsCollector& calls = callsCollectors->GetAt(i);
			GLORY_TEST_COMPARE(calls.m_OnAddCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_OnRemoveCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_OnValidateCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_OnActivateCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_OnDeactivateCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_OnEnableDrawCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_OnDisableDrawCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_OnDirtyCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_StartCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_StopCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_PreUpdateCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_UpdateCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_PostUpdateCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_PreDrawCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_DrawCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_PostDrawCalls, 0ull);
		}

		registry.Validate();
		registry.Activate();
		registry.Deactivate();
		registry.EnableDraw();
		registry.DisableDraw();
		registry.Dirty();
		registry.Start();
		registry.Stop();
		registry.Update(1.0f);
		registry.Draw();

		for (size_t i = 0; i < entityCount; ++i)
		{
			CallsCollector& calls = callsCollectors->GetAt(i);
			GLORY_TEST_COMPARE(calls.m_OnAddCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_OnRemoveCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_OnValidateCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_OnActivateCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_OnDeactivateCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_OnEnableDrawCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_OnDisableDrawCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_OnDirtyCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_StartCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_StopCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_PreUpdateCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_UpdateCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_PostUpdateCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_PreDrawCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_DrawCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_PostDrawCalls, 1ull);
		}

		/* Deactivating some components would block some calls */
		const std::array<Utils::ECS::EntityID, 12> toDisable = {
			1ull, 23ull, 25ull, 43ull, 44ull, 49ull, 60ull, 65ull, 67ull, 71ull, 90ull, 95ull
		};

		/* Deactivating will call both OnDeactivate and OnDisableDraw */
		for (auto entity : toDisable)
			callsCollectors->Deactivate(entity);

		/* Validate will cause a sort */
		registry.Validate();
		registry.Activate();
		registry.EnableDraw();
		registry.Dirty();
		registry.Start();
		registry.Stop();
		registry.Update(1.0f);
		registry.Draw();

		for (size_t i = 0; i < entityCount; ++i)
		{
			const Utils::ECS::EntityID entity = i + 1;
			const bool disabled = std::find(toDisable.begin(), toDisable.end(), entity) != toDisable.end();

			CallsCollector& calls = callsCollectors->Get(entity);
			GLORY_TEST_COMPARE(calls.m_OnAddCalls, 1ull);
			GLORY_TEST_COMPARE(calls.m_OnRemoveCalls, 0ull);
			GLORY_TEST_COMPARE(calls.m_OnValidateCalls, 2ull);
			GLORY_TEST_COMPARE(calls.m_OnActivateCalls, disabled ? 1ull : 2ull);
			GLORY_TEST_COMPARE(calls.m_OnDeactivateCalls, disabled ? 2ull : 1ull);
			GLORY_TEST_COMPARE(calls.m_OnEnableDrawCalls, disabled ? 1ull : 2ull);
			GLORY_TEST_COMPARE(calls.m_OnDisableDrawCalls, disabled ? 2ull : 1ull);
			GLORY_TEST_COMPARE(calls.m_OnDirtyCalls, 2ull);
			GLORY_TEST_COMPARE(calls.m_StartCalls, 2ull);
			GLORY_TEST_COMPARE(calls.m_StopCalls, 2ull);
			GLORY_TEST_COMPARE(calls.m_PreUpdateCalls, disabled ? 1ull : 2ull);
			GLORY_TEST_COMPARE(calls.m_UpdateCalls, disabled ? 1ull : 2ull);
			GLORY_TEST_COMPARE(calls.m_PostUpdateCalls, disabled ? 1ull : 2ull);
			GLORY_TEST_COMPARE(calls.m_PreDrawCalls, disabled ? 1ull : 2ull);
			GLORY_TEST_COMPARE(calls.m_DrawCalls, disabled ? 1ull : 2ull);
			GLORY_TEST_COMPARE(calls.m_PostDrawCalls, disabled ? 1ull : 2ull);
		}
	}

	void ECSTest::Hierarchy()
	{
		struct HierarchyTest {};

		class HierarchyTestManager : public Utils::ECS::ComponentManager<HierarchyTest>
		{
		public:
			HierarchyTestManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=100):
				ComponentManager(pRegistry, capacity) {
			};
			virtual ~HierarchyTestManager() = default;

			void DoUpdateImpl(Utils::ECS::EntityID entity, HierarchyTest&, float)
			{
				updateCallback(entity);
			}

			virtual void OnInitialize() override
			{
				Bind(DoUpdate, &HierarchyTestManager::DoUpdateImpl);
			}

			std::function<void(Utils::ECS::EntityID)> updateCallback;
		};

		Utils::ECS::EntityRegistry registry;
		HierarchyTestManager& hierarchyTestManager = registry.AddManager<HierarchyTestManager>();

		std::vector<Utils::ECS::EntityID> expectedOrder;
		size_t updateCount = 0;

		hierarchyTestManager.updateCallback = [this, &expectedOrder, &updateCount](Utils::ECS::EntityID entity) {
			GLORY_TEST_COMPARE(expectedOrder[updateCount], entity);
			++updateCount;
		};

		std::array<Utils::ECS::EntityID, 10> root1Children;
		Utils::ECS::EntityID root1 = registry.CreateEntity();
		registry.AddComponent<HierarchyTest>(root1, UUID());
		expectedOrder.push_back(root1);

		/* Ideal child order */
		for (size_t i = 0; i < 10; ++i)
		{
			Utils::ECS::EntityID child = registry.CreateEntity();
			registry.AddComponent<HierarchyTest>(child, UUID());
			registry.SetParent(child, root1);
			expectedOrder.push_back(child);
			root1Children[i] = child;

			GLORY_TEST_COMPARE(registry.ChildCount(root1), i + 1);
			GLORY_TEST_COMPARE(registry.Child(root1, i), child);
			GLORY_TEST_COMPARE(registry.ChildCount(child), 0ull);
		}

		Utils::ECS::EntityID root2 = registry.CreateEntity();
		registry.AddComponent<HierarchyTest>(root2, UUID());
		expectedOrder.push_back(root2);

		/* Predefined child order */
		std::array<Utils::ECS::EntityID, 10> children;
		const std::array<size_t, 10> childOrder = {
			5, 1, 2, 7, 0, 9, 3, 6, 4, 8
		};

		for (size_t i = 0; i < 10; ++i)
		{
			children[i] = registry.CreateEntity();
			registry.AddComponent<HierarchyTest>(children[i], UUID());

			Utils::ECS::EntityID child = registry.CreateEntity();
			registry.AddComponent<HierarchyTest>(child, UUID());
			registry.SetParent(child, children[i]);

			GLORY_TEST_COMPARE(registry.ChildCount(children[i]), 1ull);
			GLORY_TEST_COMPARE(registry.Child(children[i], 0), child);
			GLORY_TEST_COMPARE(registry.ChildCount(child), 0ull);
		}

		std::array<Utils::ECS::EntityID, 10> root2Children;
		for (size_t i = 0; i < 10; ++i)
		{
			size_t index = childOrder[i];

			registry.SetParent(children[index], root2);
			expectedOrder.push_back(children[index]);
			expectedOrder.push_back(registry.Child(children[index], 0));
			root2Children[i] = children[index];
		}

		GLORY_TEST_COMPARE(registry.ChildCount(root2), childOrder.size());
		for (size_t i = 0; i < 10; ++i)
		{
			GLORY_TEST_COMPARE(registry.Child(root2, i), children[childOrder[i]]);
		}

		registry.Update(1.0f);

		/* Test entity trees */
		GLORY_TEST_COMPARE(registry.ChildCount(0), 2ull);
		GLORY_TEST_COMPARE(registry.Child(0, 0), root1);
		GLORY_TEST_COMPARE(registry.Child(0, 1), root2);
		GLORY_TEST_COMPARE(registry.ChildCount(root1), 10ull);
		GLORY_TEST_COMPARE(registry.ChildCount(root2), 10ull);

		for (size_t i = 0; i < root1Children.size(); ++i)
		{
			GLORY_TEST_COMPARE(registry.Child(root1, i), root1Children[i]);
		}

		for (size_t i = 0; i < root2Children.size(); ++i)
		{
			const Utils::ECS::EntityID child = registry.Child(root2, i);
			GLORY_TEST_COMPARE(child, root2Children[i]);
			GLORY_TEST_COMPARE(registry.ChildCount(child), 1ull);
		}

		/* Change sibling indices */
		registry.SetSiblingIndex(root1, 1);
		GLORY_TEST_COMPARE(registry.ChildCount(0), 2ull);
		GLORY_TEST_COMPARE(registry.Child(0, 0), root2);
		GLORY_TEST_COMPARE(registry.Child(0, 1), root1);
		GLORY_TEST_COMPARE(registry.ChildCount(root1), 10ull);
		GLORY_TEST_COMPARE(registry.ChildCount(root2), 10ull);

		std::array<Utils::ECS::EntityID, 10> root2ChildrenReordered;
		for (size_t i = 1; i < root2ChildrenReordered.size(); ++i)
		{
			root2ChildrenReordered[i - 1] = root2Children[i];
		}
		root2ChildrenReordered[9] = root2Children[0];

		registry.SetSiblingIndex(root2Children[0], 9);
		for (size_t i = 0; i < root2ChildrenReordered.size(); ++i)
		{
			const Utils::ECS::EntityID child = registry.Child(root2, i);
			GLORY_TEST_COMPARE(child, root2ChildrenReordered[i]);
			GLORY_TEST_COMPARE(registry.ChildCount(child), 1ull);
		}
	}

	void ECSTest::Serialize()
	{
		Utils::ECS::EntityRegistry registry;
		m_RegistryFactory.PopulateRegisry(registry);

		UUID idCounter = 1000;

		const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount/4; ++i)
		{
			auto entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, idCounter);
			idCounter = idCounter + 1;
			registry.AddComponent<Velocity>(entity, idCounter);
			idCounter = idCounter + 1;
		}

		for (size_t i = 0; i < entityCount/4; ++i)
		{
			auto entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, idCounter);
			idCounter = idCounter + 1;
			registry.SetParent(entity, i + 1);
		}

		for (size_t i = 0; i < entityCount/4; ++i)
		{
			auto entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, idCounter);
			idCounter = idCounter + 1;
			registry.AddComponent<Velocity>(entity, idCounter);
			idCounter = idCounter + 1;
			registry.SetParent(entity, i%5 + 1);
		}

		for (size_t i = 0; i < entityCount/4; ++i)
		{
			auto entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, idCounter);
			idCounter = idCounter + 1;
			registry.SetParent(entity, i%2 + 10);
		}

		registry.Sort();
		Glory::Utils::BinaryFileStream file{ "./Serialize.entities" };
		registry.Serialize(file);
		file.Close();

		GLORY_TEST_COMPARE_CUSTOM("./Serialize.entities", "../../../../../Utils/Tests/Resources/ECSSerializeTestEntities.expected",
			Utils::CompareBinaryFiles);
	}

	void ECSTest::SerializeAndDeserialize()
	{
		Utils::ECS::EntityRegistry registry1;
		m_RegistryFactory.PopulateRegisry(registry1);
		Utils::ECS::EntityRegistry registry2;
		m_RegistryFactory.PopulateRegisry(registry2);

		const size_t entityCount = 10000;
		for (size_t i = 0; i < entityCount; ++i)
		{
			auto entity = registry1.CreateEntity();
			registry1.AddComponent<Transform>(entity, Glory::UUID());
			registry1.AddComponent<Velocity>(entity, Glory::UUID());
			if (i <= 20) continue;
			Utils::ECS::EntityID parent = Utils::ECS::EntityID(std::rand() % i);
			if (parent == 0ull) continue;
			registry1.SetParent(entity, parent);
		}

		{
			registry1.Sort();
			Glory::Utils::BinaryFileStream file{ "./SerializeAndDeserialize.entities" };
			registry1.Serialize(file);
			file.Close();
		}

		{
			Glory::Utils::BinaryFileStream file{ "./SerializeAndDeserialize.entities", true };
			registry2.Deserialize(file);
			file.Close();
		}

		GLORY_TEST_VERIFY(registry1 == registry2);
	}

	void ECSTest::DestroyEntity()
	{
		Utils::ECS::EntityRegistry registry;
		m_RegistryFactory.PopulateRegisry(registry);

		const size_t entityCount = 100;
		for (size_t i = 0; i < entityCount; ++i)
		{
			auto entity = registry.CreateEntity();
			registry.AddComponent<Transform>(entity, UUID());
			registry.AddComponent<Velocity>(entity, UUID());
		}

		const std::array<size_t, 10> toDelete = {
			50ull, 1ull, 25ull, 74ull, 11ull, 12ull, 33ull, 69ull, 42ull, 8ull
		};

		auto transforms = registry.GetComponentManager<Transform>();
		auto velocities = registry.GetComponentManager<Velocity>();

		registry.Sort();
		for (size_t i = 0; i < toDelete.size(); ++i)
		{
			GLORY_TEST_VERIFY(registry.EntityValid(toDelete[i]));
			GLORY_TEST_COMPARE(registry.EntityComponentCount(toDelete[i]), 2ull);
			GLORY_TEST_VERIFY(transforms->Index(toDelete[i]) != UINT64_MAX);
			GLORY_TEST_VERIFY(velocities->Index(toDelete[i]) != UINT64_MAX);
			GLORY_TEST_COMPARE(transforms->Size(), entityCount - i);
			GLORY_TEST_COMPARE(velocities->Size(), entityCount - i);
			GLORY_TEST_COMPARE(transforms->ActiveSize(), entityCount - i);
			GLORY_TEST_COMPARE(velocities->ActiveSize(), entityCount - i);

			registry.DestroyEntity(toDelete[i]);
			GLORY_TEST_FAIL(registry.EntityValid(toDelete[i]));
			GLORY_TEST_COMPARE(registry.EntityComponentCount(toDelete[i]), 0ull);
			GLORY_TEST_FAIL(transforms->Index(toDelete[i]) != UINT64_MAX);
			GLORY_TEST_FAIL(velocities->Index(toDelete[i]) != UINT64_MAX);
			GLORY_TEST_COMPARE(transforms->Size(), entityCount - i - 1);
			GLORY_TEST_COMPARE(velocities->Size(), entityCount - i - 1);
			GLORY_TEST_COMPARE(transforms->ActiveSize(), entityCount - i - 1);
			GLORY_TEST_COMPARE(velocities->ActiveSize(), entityCount - i - 1);
		}

		/* Disabled components should not reduce ActiveSize when removed */
		const Utils::ECS::EntityID disabledEntity = 5ull;
		registry.SetActive(disabledEntity, false);
		registry.Sort();
		GLORY_TEST_COMPARE(transforms->Size(), entityCount - toDelete.size());
		GLORY_TEST_COMPARE(velocities->Size(), entityCount - toDelete.size());
		GLORY_TEST_COMPARE(transforms->ActiveSize(), entityCount - toDelete.size() - 1);
		GLORY_TEST_COMPARE(velocities->ActiveSize(), entityCount - toDelete.size() - 1);
		registry.DestroyEntity(disabledEntity);
		GLORY_TEST_COMPARE(transforms->Size(), entityCount - toDelete.size() - 1);
		GLORY_TEST_COMPARE(velocities->Size(), entityCount - toDelete.size() - 1);
		GLORY_TEST_COMPARE(transforms->ActiveSize(), entityCount - toDelete.size() - 1);
		GLORY_TEST_COMPARE(velocities->ActiveSize(), entityCount - toDelete.size() - 1);
	}
}

GLORY_TEST_MAIN(Glory::Test::ECSTest)
