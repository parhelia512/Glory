#include "CharacterControllerManager.h"
#include "JoltPhysicsModule.h"
#include "JoltCharacterManager.h"
#include "JoltShapeManager.h"
#include "Debug.h"

#include <GScene.h>
#include <SceneManager.h>

#include <Components.h>
#include <glm/gtx/matrix_decompose.hpp>

namespace Glory
{
	CharacterControllerManager::CharacterControllerManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity):
		ComponentManager(pRegistry, capacity), m_pPhysicsModule(nullptr), m_pDebug(nullptr)
	{
	}

	CharacterControllerManager::~CharacterControllerManager()
	{
	}

	void CharacterControllerManager::OnStartImpl(Utils::ECS::EntityID entity, CharacterController& pComponent)
	{

		pComponent.m_BodyID = PhysicsBody::InvalidBodyID;
		JoltCharacterManager* pCharacters = m_pPhysicsModule->GetCharacterManager();
		JoltShapeManager* pShapes = m_pPhysicsModule->GetShapeManager();

		const Shape* pShape = pComponent.m_Shape.BaseShapePointer();
		if (pShape->m_ShapeType == ShapeType::None)
		{
			m_pDebug->LogWarning("CharacterController does not have a shape!");
			return;
		}

		const Transform& transform = m_pRegistry->GetComponent<Transform>(entity);
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;
		if (!glm::decompose(transform.MatTransform, scale, rotation, translation, skew, perspective)) return;

		const UUID shapeID = pShapes->CreateShape(*pShape);
		const ShapeData* pShapeData = pShapes->GetShape(shapeID);

		GScene* pScene = m_pRegistry->GetUserData<GScene>();
		pComponent.m_CurrentLayerIndex = m_pRegistry->HasComponent<LayerComponent>(entity) ?
			m_pRegistry->GetComponent<LayerComponent>(entity).m_Layer.m_LayerIndex : 0;
		pComponent.m_ShapeID = shapeID;
		pComponent.m_CharacterID = pCharacters->CreateCharacter(pComponent.m_MaxSlopeAngle, pComponent.m_Mass, pComponent.m_Friction,
			pComponent.m_GravityFactor, pComponent.m_CurrentLayerIndex, translation, rotation, *pShapeData);
		pComponent.m_BodyID = pCharacters->GetBodyID(pComponent.m_CharacterID);
		const UUID entityUUID = pScene->GetEntityUUID(entity);
		m_pPhysicsModule->SetBodyUserData(pComponent.m_BodyID, entityUUID);
		m_pPhysicsModule->AddToSceneIDsCache(entityUUID, pScene->GetUUID());
	}

	void CharacterControllerManager::OnStopImpl(Utils::ECS::EntityID entity, CharacterController& pComponent)
	{
		if (!pComponent.m_CharacterID) return;

		JoltCharacterManager* pCharacters = m_pPhysicsModule->GetCharacterManager();

		GScene* pScene = m_pRegistry->GetUserData<GScene>();
		pCharacters->DestroyCharacter(pComponent.m_CharacterID);
		m_pPhysicsModule->RemoveFromSceneIDsCache(pScene->GetEntityUUID(entity));
		pComponent.m_CharacterID = 0;
		pComponent.m_BodyID = 0;
		pComponent.m_ShapeID = 0;
	}

	void CharacterControllerManager::OnUpdateImpl(Utils::ECS::EntityID entity, CharacterController& pComponent, float)
	{
		if (!pComponent.m_CharacterID) return;

		JoltCharacterManager* pCharacters = m_pPhysicsModule->GetCharacterManager();
		if (!pCharacters) return;
		
		Transform& transform = m_pRegistry->GetComponent<Transform>(entity);
		transform.Position = pCharacters->GetPosition(pComponent.m_CharacterID);
		transform.Rotation = pCharacters->GetRotation(pComponent.m_CharacterID);

		const Utils::ECS::EntityID parent = m_pRegistry->GetParent(entity);
		if (m_pRegistry->EntityValid(parent))
		{
			Transform& parentTransform = m_pRegistry->GetComponent<Transform>(parent);
			const glm::mat4 inverse = glm::inverse(parentTransform.MatTransform);
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			if (!glm::decompose(inverse, scale, rotation, translation, skew, perspective)) return;
			transform.Position = inverse * glm::vec4(transform.Position, 1.0f);
			transform.Rotation = transform.Rotation * glm::conjugate(rotation);
		}

		if (m_pRegistry->HasComponent<LayerComponent>(entity))
		{
			const uint32_t layerIndex = m_pRegistry->GetComponent<LayerComponent>(entity).m_Layer.m_LayerIndex;
			if (pComponent.m_CurrentLayerIndex != layerIndex)
			{
				pCharacters->SetLayer(pComponent.m_CharacterID, layerIndex);
				pComponent.m_CurrentLayerIndex = layerIndex;
			}
		}
		else
		{
			if (pComponent.m_CurrentLayerIndex != 0)
			{
				pCharacters->SetLayer(pComponent.m_CharacterID, 0);
				pComponent.m_CurrentLayerIndex = 0;
			}
		}

		m_pRegistry->SetEntityDirty(entity);
	}

	void CharacterControllerManager::OnInitialize()
	{
		Bind(DoStart, &CharacterControllerManager::OnStartImpl);
		Bind(DoStop, &CharacterControllerManager::OnStopImpl);
		Bind(DoUpdate, &CharacterControllerManager::OnUpdateImpl);
	}
}
