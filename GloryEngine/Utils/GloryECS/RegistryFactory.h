#pragma once
#include "ECSTypeTraits.h"
#include "EntityRegistry.h"

#include <vector>
#include <memory>
#include <functional>

namespace Glory::Utils::ECS
{
	class IComponentManager;

	class ComponentManagerFactoryBase
	{
	public:
		virtual IComponentManager* Create(EntityRegistry* pRegistry) const = 0;
	};

	template<IsComponentManager Manager>
	class ComponentManagerFactory : public ComponentManagerFactoryBase
	{
	public:
		ComponentManagerFactory(std::function<void(EntityRegistry*, Manager*)> createCallback):
			m_CreateCallback(createCallback) {}
		virtual ~ComponentManagerFactory() = default;

		IComponentManager* Create(EntityRegistry* pRegistry) const override
		{
			Manager* pManager = new Manager(pRegistry);
			if (m_CreateCallback) m_CreateCallback(pRegistry, pManager);
			return pManager;
		}

	private:
		std::function<void(EntityRegistry*, Manager*)> m_CreateCallback;
	};

	class RegistryFactory
	{
	public:
		template<IsComponentManager Manager>
		void RegisterComponentManager(std::function<void(EntityRegistry*, Manager*)> createCallback=NULL)
		{
			m_ComponentManagerFactories.emplace_back(new ComponentManagerFactory<Manager>(createCallback));
		}

		void PopulateRegisry(EntityRegistry& registry) const
		{
			for (const auto& factory : m_ComponentManagerFactories)
				registry.AddManager(factory->Create(&registry));
		}

	private:
		std::vector<std::unique_ptr<ComponentManagerFactoryBase>> m_ComponentManagerFactories;
	};
}
