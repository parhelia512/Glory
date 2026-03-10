#pragma once
#include <PhysicsComponents.h>

#include <ComponentManager.h>

namespace Glory::Utils::ECS
{
    class EntityRegistry;
}

namespace Glory
{
    class IEngine;
    class JoltPhysicsModule;
    class Debug;

    class PhysicsBodyManager : public Utils::ECS::ComponentManager<PhysicsBody>
    {
    public:
        PhysicsBodyManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=100);
        virtual ~PhysicsBodyManager();

    public:
        void OnStartImpl(Utils::ECS::EntityID entity, PhysicsBody& pComponent);
        void OnStopImpl(Utils::ECS::EntityID entity, PhysicsBody& pComponent);
        void OnValidateImpl(Utils::ECS::EntityID entity, PhysicsBody& pComponent);
        void OnUpdateImpl(Utils::ECS::EntityID entity, PhysicsBody& pComponent, float);

    private:
        void OnInitialize() override;
        void SetupBody(Utils::ECS::EntityID entity, PhysicsBody& pComponent);
        
    private:
        friend class JoltPhysicsModule;
        JoltPhysicsModule* m_pPhysicsModule;
        Debug* m_pDebug;
    };
}