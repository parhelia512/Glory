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
        virtual ~TextFileData() = default;
    };
}