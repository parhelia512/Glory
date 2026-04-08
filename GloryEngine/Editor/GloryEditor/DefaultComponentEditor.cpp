#include "DefaultComponentEditor.h"
#include "PropertyDrawer.h"
#include "Undo.h"
#include "EditorUI.h"

#include <imgui.h>
#include <TypeData.h>

namespace Glory::Editor
{
	DefaultComponentEditor::DefaultComponentEditor()
	{
	}

	DefaultComponentEditor::~DefaultComponentEditor()
	{
	}

	bool DefaultComponentEditor::OnGUI()
	{
		Undo::StartRecord("Property Change", m_pTarget->GetUUID(), true);
		bool change = false;
		EntityComponentObject* pComponentObject = (EntityComponentObject*)GetTarget();
		const uint32_t hash = pComponentObject->ComponentType();
		Utils::ECS::EntityRegistry* pRegistry = pComponentObject->GetRegistry();
		const Utils::ECS::EntityID entity = pComponentObject->EntityID();
		const UUID componentID = pComponentObject->GetUUID();

		const TypeData* pTypeData = Reflect::GetTyeData(hash);

		Utils::ECS::IComponentManager* manager = pRegistry->GetComponentManager(hash);
		bool active = manager->IsActive(entity);
		if (EditorUI::CheckBox("Active", &active))
		{
			if (active)
				manager->Activate(entity);
			else
				manager->Deactivate(entity);
			change = true;
		}

		void* pAddress = pRegistry->GetComponentAddress(entity, hash);

		if (pTypeData)
		{
			change |= PropertyDrawer::DrawProperty("", pTypeData, pAddress, 0);
		}

		Undo::StopRecord();

		if (change)
			manager->CallValidate(entity);

		return change;
	}

	void DefaultComponentEditor::Initialize()
	{
		EntityComponentObject* pEntityComponentObject = (EntityComponentObject*)m_pTarget;
		const TypeData* pTypeData = Reflect::GetTyeData(pEntityComponentObject->ComponentType());
		m_Name = pTypeData->TypeName();
	}

	std::string DefaultComponentEditor::Name()
	{
		return m_Name;
	}
}
