#pragma once
#include "PhysicsComponents.h"

#include <ComponentManager.h>

namespace Glory::Utils::ECS
{
    class EntityRegistry;
}

namespace Glory
{
    class JoltPhysicsModule;
    class Debug;

    class CharacterControllerManager : public Utils::ECS::ComponentManager<CharacterController>
    {
    public:
        CharacterControllerManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=10);
        virtual ~CharacterControllerManager();

    public:
        void OnStartImpl(Utils::ECS::EntityID entity, CharacterController& pComponent);
        void OnStopImpl(Utils::ECS::EntityID entity, CharacterController& pComponent);
        void OnUpdateImpl(Utils::ECS::EntityID entity, CharacterController& pComponent, float);

    private:
        void OnInitialize() override;

    private:
        friend class JoltPhysicsModule;
        JoltPhysicsModule* m_pPhysicsModule;
        Debug* m_pDebug;
    };
}