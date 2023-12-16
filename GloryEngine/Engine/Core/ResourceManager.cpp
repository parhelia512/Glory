#include "ResourceManager.h"

namespace Glory
{
    size_t BaseResourceManager::Index(UUID uuid) const
    {
        const auto itor = std::find(m_UUIDs.begin(), m_UUIDs.end(), uuid);
        if (itor == m_UUIDs.end())
            return UINT64_MAX;

        return itor - m_UUIDs.begin();
    }

    bool BaseResourceManager::IsLoaded(UUID uuid) const
    {
        return Index(uuid) != UINT64_MAX;
    }
}
