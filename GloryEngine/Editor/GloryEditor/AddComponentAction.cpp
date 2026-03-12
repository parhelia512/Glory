#include "AddComponentAction.h"
#include "EditorSceneManager.h"
#include "EditorApplication.h"

#include <GScene.h>

namespace Glory::Editor
{
	AddComponentAction::AddComponentAction(uint32_t typeHash, UUID uuid, size_t componentIndex) : m_ComponentTypeHash(typeHash), m_ComponentUUID(uuid), m_ComponentIndex(componentIndex)
	{
	}

	AddComponentAction::~AddComponentAction()
	{
	}

	void AddComponentAction::OnUndo(const ActionRecord& actionRecord)
	{
		std::vector<Editor*> editors = Editor::FindEditors(actionRecord.ObjectID);
		if (!editors.size()) return;

		EditableEntity* pEntityObject = (EditableEntity*)editors[0]->GetTarget();
		GScene* pScene = EditorApplication::GetInstance()->GetSceneManager().GetOpenScene(pEntityObject->SceneID());
		pScene->GetRegistry().RemoveComponentAt(pEntityObject->EntityID(), m_ComponentIndex);

		for (size_t i = 0; i < editors.size(); i++)
		{
			editors[i]->Initialize();
		}
	}

	void AddComponentAction::OnRedo(const ActionRecord& actionRecord)
	{
		std::vector<Editor*> editors = Editor::FindEditors(actionRecord.ObjectID);
		if (!editors.size()) return;

		EditableEntity* pEntityObject = (EditableEntity*)editors[0]->GetTarget();
		GScene* pScene = EditorApplication::GetInstance()->GetSceneManager().GetOpenScene(pEntityObject->SceneID());
		Glory::Utils::ECS::EntityRegistry& pRegistry = pScene->GetRegistry();
		Utils::ECS::EntityID entity = pEntityObject->EntityID();
		pRegistry.CreateComponent(entity, m_ComponentTypeHash, m_ComponentUUID);
		pRegistry.SetComponentIndex(entity, pRegistry.EntityComponentCount(entity) - 1, m_ComponentIndex);

		for (size_t i = 0; i < editors.size(); i++)
		{
			editors[i]->Initialize();
		}
	}
}
