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

	class LightManager : public Utils::ECS::ComponentManager<LightComponent>
	{
    public:
        LightManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=1000);
        virtual ~LightManager();

    public:
        void OnDrawImpl(Utils::ECS::EntityID entity, LightComponent& pComponent);

    private:
        virtual void OnInitialize() override;

    private:
        friend class SceneManager;
        SceneManager* m_pSceneManager;
	};
}
