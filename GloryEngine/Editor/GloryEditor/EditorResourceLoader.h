#pragma once
#include "ImportedResource.h"

#include <ResourceLoader.h>

#include <filesystem>
#include <unordered_map>
#include <array>
#include <set>

namespace Glory
{
    class Debug;
}

namespace Glory::Jobs
{
    class JobManager;
}

namespace Glory::Editor
{
    struct ImportedResource;
    /** @brief Resource loader for the editor.
     *
     * Handles compiling, caching and loading of resources using a lock-free and
     * theoretically wait-free multi-threaded implementation.
     */
    class EditorResourceLoader final : public ResourceLoader
    {
    public:
        /** @brief Constructor
         * @param pJobManager Job manager to create a job pool from.
         * @param pDebug Debug logging instance.
         */
        EditorResourceLoader(Jobs::JobManager* pJobManager, Debug* pDebug);
        /** @brief Destructor */
        virtual ~EditorResourceLoader();

        /** @brief Check if any cached resource should be reimported.
         *
         * Should be run during startup to purge missing assets.
         * @todo Not yet implemented!
         */
        void CheckResourceCache();

    private:
        /** @brief Loading implementation */
        virtual void QueueLoad(UUID id) override;
        /** @brief Unloading implementation */
        virtual void QueueUnload(UUID id) override;

        /** @brief Custom processing update implementation */
        virtual void OnUpdate() override;

    private:
        /** @brief Check if a resource has a valid cache.
         * @param cachePath Cache path to assign for loading.
         * @param assetPath Asset path to assign for loading.
         * Also checks if the cache is outdated by comparing its write time with that of the original asset.
         */
        bool ResourceHasValidCache(UUID id, std::filesystem::path& cachePath, std::filesystem::path& assetPath) const;

        /** @brief Compilation job implementation.
         * @param assetPath Asset path to import.
         */
        bool CompileJob(const std::filesystem::path& assetPath);
        /** @brief Cache job implementation.
         * @param id ID of the resource being cached.
         * @param cachePath Path to store the cache file at.
         * @param pResource Resource being cached.
         */
        bool CacheJob(UUID id, const std::filesystem::path& cachePath, Resource* pResource);
        /** @brief Load resource cache job implementation.
         * @param id ID of the resource being loaded.
         * @param cachePath Path to load thje cache from.
         */
        bool LoadCacheJob(UUID id, const std::filesystem::path& cachePath);

        /** @brief Process the next imported resource. */
        void UpdateImportedResources();
        /** @brief Recursively process an imported resource.
         * @param resource Imported resource to process.
         */
        void ProcessImportedResource(ImportedResource& resource);

        /** @brief Process all currently loaded resources
         *
         * This just means moving them to the resource manager.
         */
        void UpdateLoadedResources();
        /** @brief Update the currently caching resources set. */
        void UpdateCachedResources();
        /** @brief Handle unloading of resources. */
        void HandleUnloading();

    private:
        Jobs::JobManager* m_pJobManager;
        Debug* m_pDebug;
        std::unordered_map<std::filesystem::path, uint8_t> m_AlreadyCompilingPaths;

        /* Resource importing jobs */
        uint32_t m_CurrentImportedResourcesReadIndex = 0;
        std::atomic_uint m_ImportWriteIndex = 0u;
        std::atomic_uint m_CurrentImportedResourceCount = 0u;
        static const uint32_t ImportedResourcesRingBufferSize = 1024;
        std::array<ImportedResource, ImportedResourcesRingBufferSize> m_ImportedResources;

        /* Cache loading jobs */
        uint32_t m_CurrentLoadedResourceReadIndex = 0;
        std::atomic_uint m_LoadedResourceWriteIndex = 0u;
        std::atomic_uint m_CurrentLoadedResourceCount = 0u;
        static const uint32_t LoadedResourcesRingBufferSize = 1024;
        std::array<Resource*, LoadedResourcesRingBufferSize> m_LoadedResources;

        /* Caching jobs */
        std::set<UUID> m_CachingItems;
        uint32_t m_CurrentCachedResourceReadIndex = 0;
        std::atomic_uint m_NextCachedResourceWriteIndex = 0u;
        std::atomic_uint m_CurrentCachedResourceCount = 0u;
        static const uint32_t CachedResourcesRingBufferSize = 1024;
        std::array<UUID, CachedResourcesRingBufferSize> m_CachedResources;

        /* Unloading jobs */
        std::set<UUID> m_ToUnload;
    };
}
