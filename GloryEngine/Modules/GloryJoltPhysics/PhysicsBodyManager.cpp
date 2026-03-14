#include "JoltPhysicsModule.h"
#include "PhysicsBodyManager.h"

#include <GScene.h>
#include <Components.h>
#include <Debug.h>

#include <glm/gtx/matrix_decompose.hpp>

namespace Glory
{
	PhysicsBodyManager::PhysicsBodyManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity):
		ComponentManager(pRegistry, capacity), m_pPhysicsModule(nullptr), m_pDebug(nullptr) { }

	PhysicsBodyManager::~PhysicsBodyManager()
	{
	}

	void PhysicsBodyManager::OnStartImpl(Utils::ECS::EntityID entity, PhysicsBody& pComponent)
	{
		SetupBody(entity, pComponent);
	}

	void PhysicsBodyManager::OnStopImpl(Utils::ECS::EntityID entity, PhysicsBody& pComponent)
	{
		GScene* pScene = m_pRegistry->GetUserData<GScene>();
		if (pComponent.m_BodyID == PhysicsBody::InvalidBodyID) return;
		m_pPhysicsModule->RemoveFromSceneIDsCache(pScene->GetEntityUUID(entity));
		m_pPhysicsModule->DestroyPhysicsBody(pComponent.m_BodyID);
	}

	void PhysicsBodyManager::OnValidateImpl(Utils::ECS::EntityID entity, PhysicsBody& pComponent)
	{
		GScene* pScene = m_pRegistry->GetUserData<GScene>();

		/* If no body was created the game has not started yet */
		if (pComponent.m_BodyID == PhysicsBody::InvalidBodyID) return;

		/* Destroy the body */
		m_pPhysicsModule->RemoveFromSceneIDsCache(pScene->GetEntityUUID(entity));
		m_pPhysicsModule->DestroyPhysicsBody(pComponent.m_BodyID);

		/* Create new body */
		SetupBody(entity, pComponent);
	}

	void PhysicsBodyManager::OnUpdateImpl(Utils::ECS::EntityID entity, PhysicsBody& pComponent, float)
	{
		/* An invalid body cannot be updated */
		if (pComponent.m_BodyID == PhysicsBody::InvalidBodyID) return;

		Transform& transform = m_pRegistry->GetComponent<Transform>(entity);
		m_pPhysicsModule->PollPhysicsState(pComponent.m_BodyID, &transform.Position, &transform.Rotation);
		transform.Rotation = glm::conjugate(transform.Rotation);

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
				m_pPhysicsModule->SetBodyObjectLayer(pComponent.m_BodyID, layerIndex);
				pComponent.m_CurrentLayerIndex = layerIndex;
			}
		}
		else
		{
			if (pComponent.m_CurrentLayerIndex != 0)
			{
				m_pPhysicsModule->SetBodyObjectLayer(pComponent.m_BodyID, 0);
				pComponent.m_CurrentLayerIndex = 0;
			}
		}

		m_pRegistry->SetEntityDirty(entity);
	}

	void PhysicsBodyManager::OnInitialize()
	{
		Bind(DoStart, &PhysicsBodyManager::OnStartImpl);
		Bind(DoStop, &PhysicsBodyManager::OnStopImpl);
		Bind(DoValidate, &PhysicsBodyManager::OnValidateImpl);
		Bind(DoUpdate, &PhysicsBodyManager::OnUpdateImpl);
	}

	void PhysicsBodyManager::SetupBody(Utils::ECS::EntityID entity, PhysicsBody& pComponent)
	{
		GScene* pScene = m_pRegistry->GetUserData<GScene>();

		pComponent.m_BodyID = PhysicsBody::InvalidBodyID;

		const Transform& transform = m_pRegistry->GetComponent<Transform>(entity);
		const Shape* pShape = pComponent.m_Shape.BaseShapePointer();
		if (pShape->m_ShapeType == ShapeType::None)
		{
			m_pDebug->LogWarning("PhysicsBody does not have a shape!");
			return;
		}

		pComponent.m_CurrentLayerIndex = m_pRegistry->HasComponent<LayerComponent>(entity) ?
			m_pRegistry->GetComponent<LayerComponent>(entity).m_Layer.m_LayerIndex : 0;

		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;
		if (!glm::decompose(transform.MatTransform, scale, rotation, translation, skew, perspective)) return;
		pComponent.m_BodyID = m_pPhysicsModule->CreatePhysicsBody(*pShape, translation, rotation, scale, pComponent.m_BodyType, pComponent.m_CurrentLayerIndex, pComponent.m_SimulationSettings);
		const UUID entityUUID = pScene->GetEntityUUID(entity);
		m_pPhysicsModule->SetBodyUserData(pComponent.m_BodyID, entityUUID);
		m_pPhysicsModule->AddToSceneIDsCache(entityUUID, pScene->GetUUID());
	}
}
