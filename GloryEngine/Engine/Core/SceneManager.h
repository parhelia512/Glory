#pragma once
#include <UUID.h>
#include <Reflection.h>

#include <string>
#include <vector>
#include <map>
#include <IComponentManager.h>
#include <RegistryFactory.h>
#include <glm/vec3.hpp>

namespace Glory
{
	class IEngine;
	class GScene;
	class Renderer;
	struct UUIDRemapper;

	class SceneManager
	{
	public:
		SceneManager(IEngine* pEngine);
		virtual ~SceneManager();

		void SetRenderer(Renderer* pRenderer);
		Renderer* GetRenderer() const;

		virtual GScene* NewScene(const std::string& name="Empty Scene", bool additive=false) = 0;
		void LoadScene(UUID uuid, bool additive);
		void UnloadScene(UUID uuid);
		void UnloadAllScenes();
		void LoadSceneNextFrame(UUID uuid, bool additive);

		virtual void OnLoadScene(UUID uuid) = 0;
		virtual void OnUnloadScene(GScene* pScene) = 0;
		virtual void OnUnloadAllScenes() = 0;

		Utils::ECS::RegistryFactory& GetRegistryFactory();

		template<Utils::ECS::IsComponentManager Manager, typename Component>
		void RegisterComponentManager(std::function<void(Utils::ECS::EntityRegistry*, Manager*)> createCallback=NULL)
		{
			m_RegistryFactory.RegisterComponentManager<Manager>(createCallback);
		    Reflect::RegisterType<Component>();
		}

		/** @brief Get the engine that owns this manager */
		IEngine* GetEngine();
		GScene* GetActiveScene(bool force=false);
		void SetActiveScene(GScene* pScene);

		size_t OpenScenesCount();
		GScene* GetOpenScene(size_t index);
		GScene* GetOpenScene(UUID uuid);
		size_t GetSceneIndex(UUID uuid) const;
		size_t ExternalSceneCount();
		GScene* GetExternalScene(size_t index);

		void UpdateScene(GScene* pScene, float dt) const;
		void DrawScene(GScene* pScene) const;

		void Start();
		void Stop();

		/** @brief Add an external scene */
		void AddExternalScene(GScene* pScene);
		void RemoveExternalScene(GScene* pScene);

		UUID AddSceneClosingCallback(std::function<void(UUID, UUID)> callback);
		void RemoveSceneClosingCallback(UUID id);

		UUID AddSceneObjectDestroyedCallback(std::function<void(UUID, UUID)> callback);
		void RemoveSceneObjectDestroyedCallback(UUID id);

		void OnSceneObjectDestroyed(UUID objectID, UUID sceneID);

		template<typename T>
		void SubscribeOnCopy(std::function<void(GScene*, void*, UUID, UUIDRemapper&)> callback)
		{
			const uint32_t hash = T::GetTypeData()->TypeHash();
			SubscribeOnCopy(hash, callback);
		}

		void SubscribeOnCopy(uint32_t hash, std::function<void(GScene*, void*, UUID, UUIDRemapper&)> callback);
		void TriggerOnCopy(uint32_t hash, GScene* pScene, void* data, UUID componentID, UUIDRemapper& remapper);

		void Initialize();
		void Cleanup();
		void Update(float dt);
		void Draw();

	protected:
		virtual void OnInitialize() = 0;
		virtual void OnCleanup() = 0;
		virtual void OnSetActiveScene(GScene* pActiveScene) = 0;

		GScene* CreateNewScene_Internal(const std::string& name, UUID uuid=UUID());
		
	protected:
		IEngine* m_pEngine;
		Renderer* m_pRenderer;
		std::vector<GScene*> m_pOpenScenes;
		std::vector<GScene*> m_pExternalScenes;
		std::vector<GScene*> m_pRemovedScenes;
		size_t m_ActiveSceneIndex;
		UUID m_HoveringObjectSceneID;
		UUID m_HoveringObjectID;
		glm::vec3 m_HoveringPos;
		glm::vec3 m_HoveringNormal;

		std::vector<UUID> m_ToLoadNextFrame;
		bool m_NextFrameLoadIsAdditive;

		Glory::Utils::ECS::RegistryFactory m_RegistryFactory;

		struct SceneCallback
		{
			UUID m_CallbackID;
			std::function<void(UUID, UUID)> m_Callback;
		};
		std::vector<SceneCallback> m_SceneClosedCallbacks;
		std::vector<SceneCallback> m_SceneObjectDestroyedCallbacks;
		std::map<uint32_t, std::function<void(GScene*, void*, UUID, UUIDRemapper&)>> m_OnComponentCopyCallbacks;
	};
}
