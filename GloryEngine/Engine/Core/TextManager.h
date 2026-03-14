#pragma once
#include "Components.h"

#include <ComponentManager.h>

namespace Glory
{
    class TextManager : public Utils::ECS::ComponentManager<TextComponent>
    {
    public:
        TextManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=100);
        virtual ~TextManager();

    public:
        void OnDrawImpl(Utils::ECS::EntityID entity, TextComponent& pComponent);
        void GetReferencesImpl(std::vector<UUID>& references) const;

    private:
        virtual void OnInitialize() override;

    private:
        friend class SceneManager;
        SceneManager* m_pSceneManager;
        AssetManager* m_pAssetManager;
        LayerManager* m_pLayerManager;
    };
}