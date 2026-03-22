#include "TransformManager.h"
#include "GScene.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <EntityRegistry.h>

namespace Glory
{
    TransformManager::TransformManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity):
        ComponentManager(pRegistry, capacity)
    {
    }

    TransformManager::~TransformManager()
    {
    }

    void TransformManager::OnValidateImpl(Utils::ECS::EntityID entity, Transform& pComponent)
    {
        CalculateMatrix(m_pRegistry, entity, pComponent);
    }

    void TransformManager::OnActivateImpl(Utils::ECS::EntityID entity, Transform& pComponent)
    {
        CalculateMatrix_Internal(entity, pComponent);
    }

    void TransformManager::OnStartImpl(Utils::ECS::EntityID entity, Transform& pComponent)
    {
        if (!m_pRegistry->IsEntityDirty(entity)) return;
        CalculateMatrix_Internal(entity, pComponent);
    }

    void Glory::TransformManager::OnPreUpdateImpl(Utils::ECS::EntityID entity, Transform& pComponent, float dt)
    {
        if (!m_pRegistry->IsEntityDirty(entity)) return;
        CalculateMatrix_Internal(entity, pComponent);
    }

    void TransformManager::CalculateMatrix(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entity, Transform& pComponent)
    {
        glm::mat4 startTransform = glm::identity<glm::mat4>();

        const Utils::ECS::EntityID parent = pRegistry->GetParent(entity);
        if (parent && pRegistry->EntityValid(parent))
        {
            const Transform& parentTransform = pRegistry->GetComponent<Transform>(parent);
            startTransform = parentTransform.MatTransform;
        }

        const glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), pComponent.Scale);
        const glm::mat4 rotation = glm::inverse(glm::mat4_cast(pComponent.Rotation));
        const glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), pComponent.Position);
        pComponent.MatTransform = startTransform * translation * rotation * scale;

        pRegistry->SetEntityDirty(entity, false);
    }

    void TransformManager::OnInitialize()
    {
        Bind(DoValidate, &TransformManager::OnValidateImpl);
        Bind(DoOnActivate, &TransformManager::OnActivateImpl);
        Bind(DoStart, &TransformManager::OnStartImpl);
        Bind(DoPreUpdate, &TransformManager::OnPreUpdateImpl);
    }

    void TransformManager::CalculateMatrix_Internal(Utils::ECS::EntityID entity, Transform& pComponent)
    {
        glm::mat4 startTransform = glm::identity<glm::mat4>();

        const Utils::ECS::EntityID parent = m_pRegistry->GetParent(entity);
        if (parent && m_pRegistry->EntityValid(parent))
        {
            const Transform& parentTransform = Get(parent);
            startTransform = parentTransform.MatTransform;
        }

        const glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), pComponent.Scale);
        const glm::mat4 rotation = glm::inverse(glm::mat4_cast(pComponent.Rotation));
        const glm::mat4 translation = glm::translate(glm::identity<glm::mat4>(), pComponent.Position);
        pComponent.MatTransform = startTransform * translation * rotation * scale;

        m_pRegistry->SetEntityDirty(entity, false);
    }
}
