#pragma once
#include "Components.h"

#include <ComponentManager.h>

namespace Glory
{
    class SceneManager;
    class AssetManager;
    class MaterialManager;
    class AssetDatabase;
    class LayerManager;
    class Debug;

    class MeshRenderManager : public Utils::ECS::ComponentManager<MeshRenderer>
    {
    public:
        MeshRenderManager(Utils::ECS::EntityRegistry* pRegistry, size_t capacity=100);
        virtual ~MeshRenderManager();

    public:
        void OnDirtyImpl(Utils::ECS::EntityID entity, MeshRenderer& pComponent);
        void OnDrawImpl(Utils::ECS::EntityID entity, MeshRenderer& pComponent);
        void OnEnableDrawImpl(Utils::ECS::EntityID entity, MeshRenderer& pComponent);
        void OnDisableDrawImpl(Utils::ECS::EntityID entity, MeshRenderer& pComponent);
        void OnValidateImpl(Utils::ECS::EntityID entity, MeshRenderer& pComponent);
        void GetReferencesImpl(std::vector<UUID>& references) const;

    private:
        virtual void OnInitialize() override;

    private:
        friend class SceneManager;
        SceneManager* m_pSceneManager;
        AssetManager* m_pAssetManager;
        MaterialManager* m_pMaterialManager;
        AssetDatabase* m_pAssetDatabase;
        LayerManager* m_pLayerManager;
        Debug* m_pDebug;
    };
}
