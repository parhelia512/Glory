#pragma once
#include "Components.h"

#include <ComponentManager.h>

namespace Glory::Utils::ECS
{
    class EntityRegistry;
}

namespace Glory
{
    class SceneManager;
    class CameraManager;

    class CameraComponentManager : public Utils::ECS::ComponentManager<CameraComponent>
    {
    public:
        GLORY_ENGINE_API CameraComponentManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=6);
        GLORY_ENGINE_API virtual ~CameraComponentManager();

    public:
        GLORY_ENGINE_API void OnValidateImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        GLORY_ENGINE_API void OnComponentAddedImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        GLORY_ENGINE_API void OnComponentRemovedImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        GLORY_ENGINE_API void OnUpdateImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent, float);
        GLORY_ENGINE_API void OnEnableDrawImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        GLORY_ENGINE_API void OnDisableDrawImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        GLORY_ENGINE_API static void Focus(Transform& transform, CameraComponent& pComponent, const BoundingSphere& boundingSphere);

        GLORY_ENGINE_API static std::string Name();

    private:
        virtual void OnInitialize() override;

    private:
        friend class SceneManager;
        SceneManager* m_pSceneManager;
        CameraManager* m_pCameraManager;
    };
}
