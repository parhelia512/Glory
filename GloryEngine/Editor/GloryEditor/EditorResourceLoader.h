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

    class EditorResourceLoader : public ResourceLoader
    {
    public:
        EditorResourceLoader(Jobs::JobManager* pJobManager, Debug* pDebug);
        virtual ~EditorResourceLoader();

        void CheckResourceCache();

    private:
        virtual void QueueLoad(UUID id) override;
        virtual void QueueUnload(UUID id) override;

        virtual void OnUpdate() override;

    private:
        bool ResourceHasValidCache(UUID id, std::filesystem::path& cachePath, std::filesystem::path& assetPath) const;

        bool CompileJob(UUID id, const std::filesystem::path& assetPath);
        bool CacheJob(UUID id, const std::filesystem::path& cachePath, Resource* pResource);
        bool LoadCacheJob(UUID id, const std::filesystem::path& cachePath);

        void UpdateImportedResources();
        void ProcessImportedResource(ImportedResource& resource);

        void UpdateLoadedResources();
        void UpdateCachedResources();
        void HandleUnloading();

    private:
        Jobs::JobManager* m_pJobManager;
        Debug* m_pDebug;
        std::unordered_map<std::filesystem::path, uint8_t> m_AlreadyCompilingPaths;

        /* Imported resources */
        uint32_t m_CurrentImportedResourcesReadIndex = 0;
        std::atomic_uint m_ImportWriteIndex = 0u;
        std::atomic_uint m_CurrentImportedResourceCount = 0u;
        static const uint32_t ImportedResourcesRingBufferSize = 1024;
        std::array<ImportedResource, ImportedResourcesRingBufferSize> m_ImportedResources;

        /* Cache loading */
        uint32_t m_CurrentLoadedResourceReadIndex = 0;
        std::atomic_uint m_LoadedResourceWriteIndex = 0u;
        std::atomic_uint m_CurrentLoadedResourceCount = 0u;
        static const uint32_t LoadedResourcesRingBufferSize = 1024;
        std::array<Resource*, LoadedResourcesRingBufferSize> m_LoadedResources;

        /* Caching */
        std::set<UUID> m_CachingItems;
        uint32_t m_CurrentCachedResourceReadIndex = 0;
        std::atomic_uint m_NextCachedResourceWriteIndex = 0u;
        std::atomic_uint m_CurrentCachedResourceCount = 0u;
        static const uint32_t CachedResourcesRingBufferSize = 1024;
        std::array<UUID, CachedResourcesRingBufferSize> m_CachedResources;

        /* Unloading */
        std::set<UUID> m_ToUnload;
    };
}
