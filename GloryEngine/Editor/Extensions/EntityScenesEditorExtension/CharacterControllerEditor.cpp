#include "CharacterControllerEditor.h"
#include "Undo.h"
#include "GizmoAction.h"
#include <glm/gtx/quaternion.hpp>
#include <TypeData.h>
#include <glm/gtx/matrix_decompose.hpp>

namespace Glory::Editor
{
	CharacterControllerEditor::CharacterControllerEditor()
	{
	}

	CharacterControllerEditor::~CharacterControllerEditor()
	{
	}

	void CharacterControllerEditor::Initialize()
	{
		EntityComponentEditor::Initialize();
	}

	bool CharacterControllerEditor::OnGUI()
	{
		Transform& transform = m_pComponentObject->GetRegistry()->GetComponent<Transform>(m_pComponentObject->EntityID());

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transform.MatTransform, scale, rotation, translation, skew, perspective);

		CharacterController& body = GetTargetComponent();

		Game::GetGame().GetEngine()->GetRendererModule()->DrawLineShape(transform.MatTransform, {}, body.m_Shape, {1, 0, 0, 1});
		return EntityComponentEditor::OnGUI();
	}

	std::string CharacterControllerEditor::Name()
	{
		return "Character Controller";
	}
}