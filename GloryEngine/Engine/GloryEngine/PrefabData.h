#pragma once
#include "GScene.h"

#include <engine_visibility.h>

#include <EntityID.h>
#include <EntityRegistry.h>

namespace Glory
{
    class PrefabData;
    class Entity;

    class PrefabData : public GScene
    {
    public:
        GLORY_ENGINE_API PrefabData();
        /** @overload */
        GLORY_ENGINE_API PrefabData(PrefabData&&) noexcept = default;
        /** @brief Move assignment operator */
        GLORY_ENGINE_API PrefabData& operator=(PrefabData&&) noexcept = default;
        GLORY_ENGINE_API virtual ~PrefabData() = default;
        GLORY_ENGINE_API static PrefabData* CreateFromEntity(GScene* pScene, Utils::ECS::EntityID entity);

    private:
        static void CopyEntity(PrefabData* pPrefab, GScene* pScene, Utils::ECS::EntityID entity, Utils::ECS::EntityID parent);

        /** @brief Get a vector containing other resources referenced by this resource */
        virtual void References(IEngine*, std::vector<UUID>& references) const override;
    };
}
