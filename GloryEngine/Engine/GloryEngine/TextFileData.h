#pragma once
#include "FileData.h"

#include <engine_visibility.h>

namespace Glory
{
    class TextFileData : public FileData
    {
    public:
        GLORY_ENGINE_API TextFileData();
        GLORY_ENGINE_API TextFileData(FileData* pFileData);
        GLORY_ENGINE_API TextFileData(std::vector<char>&& data);
        GLORY_ENGINE_API TextFileData(std::string_view data);
        GLORY_ENGINE_API TextFileData(TextFileData&&) noexcept = default;
        GLORY_ENGINE_API TextFileData& operator=(TextFileData&&) noexcept = default;
        virtual ~TextFileData() = default;
    };
}