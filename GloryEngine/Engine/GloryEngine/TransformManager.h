#pragma once
#include "Components.h"

#include <ComponentManager.h>

namespace Glory::Utils::ECS
{
    class EntityRegistry;
}

namespace Glory
{
    class TransformManager : public Glory::Utils::ECS::ComponentManager<Transform>
    {
    public:
        GLORY_ENGINE_API TransformManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=1000);
        GLORY_ENGINE_API virtual ~TransformManager();

    public:
        void OnValidateImpl(Utils::ECS::EntityID entity, Transform& pComponent);
        void OnActivateImpl(Utils::ECS::EntityID entity, Transform& pComponent);
        void OnStartImpl(Utils::ECS::EntityID entity, Transform& pComponent);
        void OnPreUpdateImpl(Utils::ECS::EntityID entity, Transform& pComponent, float dt);

        static void CalculateMatrix(Utils::ECS::EntityRegistry* pRegistry, Utils::ECS::EntityID entity, Transform& pComponent);

    private:
        void OnInitialize() override;
        void CalculateMatrix_Internal(Utils::ECS::EntityID entity, Transform& pComponent);
    };
}
