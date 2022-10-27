#include <glm/glm.hpp>
#include <JobManager.h>
#include "EntityScene.h"
#include "Entity.h"
#include "Components.h"
#include "Systems.h"
#include "EntitySceneObject.h"




#include <Game.h>
#include <Engine.h>

namespace Glory
{
	EntityScene::EntityScene() : GScene("New Scene") //: m_pJobPool(nullptr)
	{
		APPEND_TYPE(EntityScene);
	}

	EntityScene::EntityScene(const std::string& sceneName) : GScene(sceneName)
	{
		APPEND_TYPE(EntityScene);
	}

	EntityScene::EntityScene(const std::string& sceneName, UUID uuid) : GScene(sceneName, uuid)
	{
		APPEND_TYPE(EntityScene);
	}

	EntityScene::~EntityScene()
	{
	}

	Entity EntityScene::CreateEntity()
	{
		EntityID entityID = m_Registry.CreateEntity<Transform>();
		return Entity(entityID, this);
	}

	EntitySceneObject* EntityScene::GetEntitySceneObjectFromEntityID(EntityID entity)
	{
		if (m_EntityIDToObject.find(entity) == m_EntityIDToObject.end()) return nullptr;
		return m_EntityIDToObject[entity];
	}

	EntityRegistry* EntityScene::GetRegistry()
	{
		return &m_Registry;
	}

	void EntityScene::Initialize()
	{
		// Register engine systems
		m_Registry.RegisterComponent<Transform>();
		//m_Registry.RegisterSystem<MeshRenderSystem>();
		//m_Registry.RegisterSystem<MeshFilterSystem>();
		m_Registry.RegisterComponent<CameraComponent>();
		//m_Registry.RegisterSystem<LookAtSystem>();
		//m_Registry.RegisterSystem<SpinSystem>();
		//m_Registry.RegisterSystem<LightSystem>();
	}

	void EntityScene::OnTick()
	{
		//m_Registry.Update();
		//while (m_Scene.m_Registry.IsUpdating()) {}
	}

	void EntityScene::OnPaint()
	{
		//m_Registry.Draw();
		//while (m_Scene.m_Registry.IsUpdating()) {}
	}

	SceneObject* EntityScene::CreateObject(const std::string& name)
	{
		Entity entity = CreateEntity();
		return new EntitySceneObject(entity, name);
	}

	SceneObject* EntityScene::CreateObject(const std::string& name, UUID uuid)
	{
		Entity entity = CreateEntity();
		return new EntitySceneObject(entity, name, uuid);
	}

	void EntityScene::OnDeleteObject(SceneObject* pObject)
	{
		
	}

	void EntityScene::OnObjectAdded(SceneObject* pObject)
	{
		EntitySceneObject* pEntityObject = (EntitySceneObject*)pObject;
		EntityID entity = pEntityObject->GetEntityHandle().GetEntityID();
		m_EntityIDToObject[entity] = pEntityObject;
	}
}