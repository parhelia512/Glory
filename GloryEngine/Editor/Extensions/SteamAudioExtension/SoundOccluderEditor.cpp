#include "SoundOccluderEditor.h"

#include <EditorApplication.h>
#include <EditorResourceManager.h>

namespace Glory::Editor
{
	SoundOccluderEditor::SoundOccluderEditor(): m_pSoundMaterialEditor(nullptr)
	{
	}

	SoundOccluderEditor::~SoundOccluderEditor()
	{
		if (!m_pSoundMaterialEditor) return;
		Editor::ReleaseEditor(m_pSoundMaterialEditor);
		m_pSoundMaterialEditor = nullptr;
	}

	void SoundOccluderEditor::Initialize()
	{
		EntityComponentEditor::Initialize();
	}

	bool SoundOccluderEditor::OnGUI()
	{
		Undo::StartRecord("Property Change", m_pComponentObject->GetUUID(), true);
		bool change = false;
		const TypeData* pTypeData = SoundOccluder::GetTypeData();
		const FieldData* pMaterialAssetField = pTypeData->GetFieldData("m_MaterialAsset");
		const FieldData* pMaterialField = pTypeData->GetFieldData("m_Material");

		const Utils::ECS::EntityID entity = m_pComponentObject->EntityID();
		Utils::ECS::IComponentManager* manager = m_pComponentObject->GetRegistry()->GetComponentManager<SoundOccluder>();
		bool active = manager->IsActive(entity);
		if (EditorUI::CheckBox("Active", &active))
		{
			if (active)
				manager->Activate(entity);
			else
				manager->Deactivate(entity);
			change = true;
		}

		SoundOccluder& occluder = GetTargetComponent();
		PropertyDrawer::DrawProperty(pMaterialAssetField, &occluder.m_MaterialAsset, 0);

		const bool hasMaterialAsset = occluder.m_MaterialAsset.GetUUID() != 0;
		ImGui::BeginDisabled(hasMaterialAsset);
		if (!hasMaterialAsset)
			PropertyDrawer::DrawProperty(pMaterialField, &occluder.m_Material, 0);
		else
		{
			EditableResource* pResource = EditorApplication::GetInstance()->GetResourceManager().GetEditableResource(occluder.m_MaterialAsset.GetUUID());

			if (pResource)
			{
				if (!m_pSoundMaterialEditor)
					m_pSoundMaterialEditor = Editor::CreateEditor((Object*)pResource);
				m_pSoundMaterialEditor->SetTarget((Object*)pResource);

				m_pSoundMaterialEditor->OnGUI();
			}
		}
		ImGui::EndDisabled();

		Undo::StopRecord();

		if (change) Validate();
		return change;


		return EntityComponentEditor::OnGUI();
	}

	std::string SoundOccluderEditor::Name()
	{
		return "Sound Occluder";
	}
}
