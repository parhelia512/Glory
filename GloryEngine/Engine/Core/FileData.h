#pragma once
#include "Resource.h"

#include <engine_visibility.h>

#include <vector>

namespace Glory
{
    class FileData : public Resource
    {
    public:
        GLORY_ENGINE_API FileData();
        GLORY_ENGINE_API FileData(FileData* pFileData);
        GLORY_ENGINE_API FileData(std::vector<char>&& data);
        GLORY_ENGINE_API FileData(const std::vector<char>& data);
        GLORY_ENGINE_API FileData(std::string_view data);
        GLORY_ENGINE_API virtual ~FileData();

        GLORY_ENGINE_API size_t Size() const;
        GLORY_ENGINE_API const char* Data() const;

        GLORY_ENGINE_API std::vector<char>::iterator Begin();
        GLORY_ENGINE_API const std::vector<char>::const_iterator Begin() const;
        GLORY_ENGINE_API std::vector<char>::iterator End();
        GLORY_ENGINE_API const std::vector<char>::const_iterator End() const;

        GLORY_ENGINE_API void Serialize(Utils::BinaryStream& container) const override;
        GLORY_ENGINE_API void Deserialize(Utils::BinaryStream& container) override;

        GLORY_ENGINE_API void SetMetaData(const void* data, size_t size);

        template<typename T>
        inline void SetMetaData(const T& value)
        {
            SetMetaData(&value, sizeof(T));
        }

        template<typename T>
        inline const T& GetMetaData() const
        {
            return *reinterpret_cast<const T*>(m_MetaData.data());
        }

    private:
        virtual void References(IEngine*, std::vector<UUID>&) const override {}

    private:
        friend class FileLoaderModule;
        std::vector<char> m_Data;
        std::vector<char> m_MetaData;
    };
}
