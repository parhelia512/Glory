#include "RemoveComponentAction.h"
#include "EditorSceneSerializer.h"
#include "EditableEntity.h"
#include "EditorSceneManager.h"
#include "Editor.h"
#include "EditorApplication.h"

#include <SceneManager.h>
#include <IEngine.h>
#include <UUIDRemapper.h>

#include <EntityRegistry.h>

namespace Glory::Editor
{
	RemoveComponentAction::RemoveComponentAction(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entityID, size_t componentIndex) : m_ComponentIndex(componentIndex)
	{
		EditorSceneSerializer::SerializeComponent(EditorApplication::GetInstance(), pRegistry, entityID, componentIndex, m_SerializedComponent.RootNodeRef().ValueRef());
	}

	RemoveComponentAction::~RemoveComponentAction()
	{
	}

	void RemoveComponentAction::OnUndo(const ActionRecord& actionRecord)
	{
		std::vector<Editor*> editors = Editor::FindEditors(actionRecord.ObjectID);
		if (!editors.size()) return;

		EditableEntity* pEntityObject = (EditableEntity*)editors[0]->GetTarget();
		GScene* pEntityScene = EditorApplication::GetInstance()->GetSceneManager().GetOpenScene(pEntityObject->SceneID());
		const size_t index = pEntityScene->GetRegistry().EntityComponentCount(pEntityObject->EntityID());
		UUIDRemapper remapper{};
		EditorSceneSerializer::DeserializeComponent(EditorApplication::GetInstance(), pEntityScene, pEntityObject->EntityID(), remapper, m_SerializedComponent.RootNodeRef().ValueRef());
		pEntityScene->GetRegistry().SetComponentIndex(pEntityObject->EntityID(), index, m_ComponentIndex);

		for (size_t i = 0; i < editors.size(); ++i)
		{
			editors[i]->Initialize();
		}
	}

	void RemoveComponentAction::OnRedo(const ActionRecord& actionRecord)
	{
		std::vector<Editor*> editors = Editor::FindEditors(actionRecord.ObjectID);
		if (!editors.size()) return;

		EditableEntity* pEntityObject = (EditableEntity*)editors[0]->GetTarget();
		GScene* pEntityScene = EditorApplication::GetInstance()->GetSceneManager().GetOpenScene(pEntityObject->SceneID());
		const uint32_t type = pEntityScene->GetRegistry().EntityComponentType(pEntityObject->EntityID(), m_ComponentIndex);
		pEntityScene->GetRegistry().RemoveComponent(pEntityObject->EntityID(), type);

		for (size_t i = 0; i < editors.size(); i++)
		{
			editors[i]->Initialize();
		}
	}
}
