#pragma once
#include <Object.h>

#include <engine_visibility.h>

#include <map>
#include <string>
#include <string_view>
#include <filesystem>

namespace Glory::Utils
{
    class BinaryStream;
}

namespace Glory
{
    class IEngine;

    /** @brief Resource */
    class Resource : public Object
    {
    public:
        /** @brief Constructor */
        GLORY_ENGINE_API Resource();
        /** @overload */
        GLORY_ENGINE_API Resource(UUID uuid);
        /** @overload */
        GLORY_ENGINE_API Resource(const std::string& name);
        /** @overload */
        GLORY_ENGINE_API Resource(UUID uuid, const std::string& name);
        /** @brief Destructor */
        GLORY_ENGINE_API virtual ~Resource();

        /** @brief Whether the resource is dirty */
        GLORY_ENGINE_API bool IsDirty() const;
        /** @brief Set resource dirty */
        GLORY_ENGINE_API void SetDirty(bool dirty);

    public:
        virtual void Serialize(Utils::BinaryStream& container) const {};
        virtual void Deserialize(Utils::BinaryStream& container) {};

        /** @brief Change the UUID of this resource
         * @param uuid The new UUID of this resource
         *
         * Only use this after constructing a new resource,
         * never use this on an existing resource!
         */
        GLORY_ENGINE_API void SetResourceUUID(UUID uuid);

        /** @brief Get a vector containing other resources referenced by this resource */
        virtual void References(IEngine* pEngine, std::vector<UUID>& references) const = 0;

    private:
        friend class AssetManager;
        friend class AssetDatabase;
        friend class LoaderModule;

        bool m_IsDirty;
    };
}
