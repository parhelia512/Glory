#pragma once
#include "engine_visibility.h"

#include "ResourceManager.h"

#include <set>
#include <functional>

namespace Glory
{
	class Debug;
	class AssetDatabase;

	class Resources
	{
	public:
		Resources(AssetDatabase* pDatabase, ResourceTypes* pResourceTypes, Debug* pDebug);
		~Resources() = default;

	public:
		template<typename R>
		requires ResourceCompatible<R>
		inline void RegisterResource(std::function<void(R*)>&& handler=NULL)
		{
			m_pResourceTypes->RegisterResource<R>("", std::move(handler));
			const size_t managerIndex = m_Managers.size();
			ResourceManager<R>* pManager = new ResourceManager<R>(this);
			m_HashToManagerIndex.emplace(pManager->TypeHash, managerIndex);
			m_Managers.emplace_back(pManager);
		}

		template<typename R>
		requires ResourceCompatible<R>
		inline bool AddResource(R** pResource)
		{
			return static_cast<R*>(AddResource(static_cast<Resource**>(pResource)));
		}

		GLORY_ENGINE_API bool AddResource(Resource** pResource);
		GLORY_ENGINE_API Resource* GetResource(UUID id);
		GLORY_ENGINE_API ResourceTypes* GetResourceTypes();

		void AddReference(UUID id);
		void RemoveReference(UUID id);

		void HandleToLoad(std::function<void(UUID)> callback);
		void HandleToUnload(std::function<void(UUID)> callback);

		GLORY_ENGINE_API void UnloadResource(UUID id);

	private:
		std::vector<std::unique_ptr<IResourceManager>> m_Managers;
		std::map<uint32_t, size_t> m_HashToManagerIndex;
		std::map<UUID, size_t> m_ResourceIDToManagerIndex;
		std::map<UUID, size_t> m_ReferenceCounter;

		std::set<UUID> m_ToLoadResources;
		std::set<UUID> m_ToUnloadResources;

		AssetDatabase* m_pDatabase;
		ResourceTypes* m_pResourceTypes;
		Debug* m_pDebug;
	};
}
