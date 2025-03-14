#include "AudioSourceEditor.h"

#include <EditorApplication.h>
#include <Audio3DModule.h>
#include <IconsFontAwesome6.h>

namespace Glory::Editor
{
	AudioSourceEditor::AudioSourceEditor()
	{
	}

	AudioSourceEditor::~AudioSourceEditor()
	{
	}

	void AudioSourceEditor::Initialize()
	{
		EntityComponentEditor::Initialize();
	}

	void DrawWarningWindow(const char* warning)
	{
		const float childHeight = ImGui::CalcTextSize("A").y * 3.5f;
		ImGui::BeginChild("warning", { 0.0f, childHeight }, true, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, ICON_FA_TRIANGLE_EXCLAMATION);
			ImGui::SameLine();
			ImGui::Text(" Warning");
			ImGui::EndMenuBar();
		}
		ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, warning);
		ImGui::EndChild();
	}

	bool AudioSourceEditor::OnGUI()
	{
		Undo::StartRecord("Property Change", m_pComponentObject->GetUUID(), true);
		bool change = false;

		const uint32_t hash = ResourceTypes::GetHash<AudioSource>();
		const TypeData* pTypeData = Reflect::GetTyeData(hash);
		AudioSource& component = GetTargetComponent();

		Audio3DModule* p3DAudio = EditorApplication::GetInstance()->GetEngine()->GetOptionalModule<Audio3DModule>();

		PropertyDrawer::SetDisabledCheckCallback([&](const Utils::Reflect::FieldData* pField) {
			if (strcmp(pField->Name(), "m_AmbisonicsOrder") == 0)
			{
				return component.m_Spatialization.m_Mode != SpatializationMode::Ambisonics;
			}
			if (strcmp(pField->Name(), "m_SpatialBlend") == 0)
			{
				return component.m_Spatialization.m_Mode != SpatializationMode::Binaural;
			}

			if (strcmp(pField->Name(), "m_LowCoefficient") == 0 ||
				strcmp(pField->Name(), "m_MidCoefficient") == 0 ||
				strcmp(pField->Name(), "m_HighCoefficient") == 0)
			{
				return component.m_Simulation.m_Direct.m_AirAbsorption.m_Type != AirAbsorptionType::Exponential;
			}

			if (strcmp(pField->Name(), "m_VolumetricRadius") == 0 ||
				strcmp(pField->Name(), "m_VolumetricSamples") == 0)
			{
				return component.m_Simulation.m_Direct.m_Occlusion.m_Type != OcclusionType::Volumetric;
			}

			if (strcmp(pField->Name(), "m_Direct") == 0)
				return !(component.m_Enable3D && !component.m_Spatialization.m_Attenuation.m_Enable
					&& p3DAudio && p3DAudio->HasFeature(Audio3DModule::Features::DirectSimulation));
			if (strcmp(pField->Name(), "m_Reflections") == 0)
				return !(component.m_Enable3D && p3DAudio && p3DAudio->HasFeature(Audio3DModule::Features::ReflectionSimulation));
			if (strcmp(pField->Name(), "m_Pathing") == 0)
				return !(component.m_Enable3D && p3DAudio && p3DAudio->HasFeature(Audio3DModule::Features::PathingSimulation));

			switch (pField->Offset())
			{
			case offsetof(AudioSource, m_Enable3D):
				return !p3DAudio;
			case offsetof(AudioSource, m_Simulation):
				return !(component.m_Enable3D && p3DAudio && p3DAudio->HasFeature(Audio3DModule::Features::Occlusion) && !component.m_Spatialization.m_Attenuation.m_Enable);
			case offsetof(AudioSource, m_Spatialization):
				return !(component.m_Enable3D && p3DAudio && p3DAudio->HasFeature(Audio3DModule::Features::Spatialization));

			default:
				return false;
			}
		});

		if (pTypeData)
		{
			const Utils::ECS::EntityID entity = m_pComponentObject->EntityID();
			Utils::ECS::BaseTypeView* pTypeView = m_pComponentObject->GetRegistry()->GetTypeView(hash);
			bool active = pTypeView->IsActive(entity);
			if (EditorUI::CheckBox("Active", &active))
			{
				pTypeView->SetActive(entity, active);
				change = true;
			}

			PropertyDrawer::DrawProperty("", pTypeData, &component, 0);
		}
		PropertyDrawer::SetDisabledCheckCallback();

		if (!p3DAudio)
			DrawWarningWindow("No 3D audio module loaded, spatialization and simulation are disabled.");
		else if(component.m_Spatialization.m_Attenuation.m_Enable)
			DrawWarningWindow("If you want to enable simulation you have to disable attenuation in the spatialization settimgs.");

		Undo::StopRecord();

		if (change) Validate();
		return change;
	}

	std::string AudioSourceEditor::Name()
	{
		return "Audio Source";
	}
}
