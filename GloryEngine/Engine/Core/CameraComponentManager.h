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
        CameraComponentManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=6);
        virtual ~CameraComponentManager();

    public:
        void OnValidateImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        void OnComponentAddedImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        void OnComponentRemovedImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        void OnUpdateImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent, float);
        void OnEnableDrawImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        void OnDisableDrawImpl(Utils::ECS::EntityID entity, CameraComponent& pComponent);
        static void Focus(Transform& transform, CameraComponent& pComponent, const BoundingSphere& boundingSphere);

        static std::string Name();

    private:
        virtual void OnInitialize() override;

    private:
        friend class SceneManager;
        SceneManager* m_pSceneManager;
        CameraManager* m_pCameraManager;
    };
}
