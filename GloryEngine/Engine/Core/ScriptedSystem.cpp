#include "ScriptedSystem.h"
#include "GScene.h"

#include "Engine.h"
#include "SceneManager.h"

namespace Glory
{
	void ScriptedSystem::OnAdd(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entity, ScriptedComponent& pComponent)
	{
		const UUID uuid = pComponent.m_Script.AssetUUID();
		if (!uuid) return;
		Script* pScript = pRegistry->GetUserData<GScene*>()->Manager()->GetEngine()->GetResources().Manager<Script>()->Get(uuid);
		if (pScript == nullptr) return;

		pScript->LoadScriptProperties(pComponent.m_ScriptProperties, pComponent.m_ScriptData);
	}

	void ScriptedSystem::OnStart(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entity, ScriptedComponent& pComponent)
	{
		const UUID uuid = pComponent.m_Script.AssetUUID();
		if (!uuid) return;
		Script* pScript = pRegistry->GetUserData<GScene*>()->Manager()->GetEngine()->GetResources().Manager<Script>()->Get(uuid);
		if (pScript == nullptr) return;
		GScene* pScene = pRegistry->GetUserData<GScene*>();
		const UUID entityUuid = pScene->GetEntityUUID(entity);

		pScript->LoadScriptProperties(pComponent.m_ScriptProperties, pComponent.m_ScriptData);
		const UUID sceneID = pRegistry->GetUserData<GScene*>()->GetUUID();
		pScript->SetPropertyValues(entityUuid, sceneID, pComponent.m_ScriptData);

		pScript->Invoke(entityUuid, sceneID, "Start", nullptr);
	}

	void ScriptedSystem::OnStop(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entity, ScriptedComponent& pComponent)
	{
		const UUID uuid = pComponent.m_Script.AssetUUID();
		if (!uuid) return;
		Script* pScript = pRegistry->GetUserData<GScene*>()->Manager()->GetEngine()->GetResources().Manager<Script>()->Get(uuid);
		if (pScript == nullptr) return;
		GScene* pScene = pRegistry->GetUserData<GScene*>();
		const UUID entityUuid = pScene->GetEntityUUID(entity);
		const UUID sceneID = pRegistry->GetUserData<GScene*>()->GetUUID();
		pScript->Invoke(entityUuid, sceneID, "Stop", nullptr);
	}

	void ScriptedSystem::OnValidate(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entity, ScriptedComponent& pComponent)
	{
		const UUID uuid = pComponent.m_Script.AssetUUID();
		if (!uuid) return;
		Script* pScript = pRegistry->GetUserData<GScene*>()->Manager()->GetEngine()->GetResources().Manager<Script>()->Get(uuid);
		if (pScript == nullptr) return;

		pScript->LoadScriptProperties(pComponent.m_ScriptProperties, pComponent.m_ScriptData);

		GScene* pScene = pRegistry->GetUserData<GScene*>();
		const UUID entityUuid = pScene->GetEntityUUID(entity);
		const UUID sceneID = pRegistry->GetUserData<GScene*>()->GetUUID();
		pScript->SetPropertyValues(entityUuid, sceneID, pComponent.m_ScriptData);
		pScript->Invoke(entityUuid, sceneID, "OnValidate", nullptr);
	}

	void ScriptedSystem::OnUpdate(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entity, ScriptedComponent& pComponent)
	{
		const UUID uuid = pComponent.m_Script.AssetUUID();
		if (!uuid) return;
		Script* pScript = pRegistry->GetUserData<GScene*>()->Manager()->GetEngine()->GetResources().Manager<Script>()->Get(uuid);
		if (pScript == nullptr) return;
		GScene* pScene = pRegistry->GetUserData<GScene*>();
		const UUID entityUuid = pScene->GetEntityUUID(entity);
		const UUID sceneID = pRegistry->GetUserData<GScene*>()->GetUUID();
		pScript->Invoke(entityUuid, sceneID, "Update", nullptr);
		pScript->GetPropertyValues(entityUuid, sceneID, pComponent.m_ScriptData);
	}

	void ScriptedSystem::OnDraw(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entity, ScriptedComponent& pComponent)
	{
		const UUID uuid = pComponent.m_Script.AssetUUID();
		if (!uuid) return;
		Script* pScript = pRegistry->GetUserData<GScene*>()->Manager()->GetEngine()->GetResources().Manager<Script>()->Get(uuid);
		if (pScript == nullptr) return;
		GScene* pScene = pRegistry->GetUserData<GScene*>();
		const UUID entityUuid = pScene->GetEntityUUID(entity);
		const UUID sceneID = pRegistry->GetUserData<GScene*>()->GetUUID();
		pScript->Invoke(entityUuid, sceneID, "Draw", nullptr);
	}
}
