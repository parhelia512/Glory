#pragma once
#include "mono_visibility.h"

#include "ScriptProperty.h"

#include <FileData.h>

#include <mono/metadata/object-forward.h>
#include <mono/metadata/class.h>

namespace YAML
{
    class Node;
}

namespace Glory
{
    class Assembly;
    struct AssemblyClass;

    class MonoScript : public FileData
    {
    public:
        GLORY_MONO_API MonoScript();
        GLORY_MONO_API MonoScript(FileData* pFileData, std::vector<std::string>&& classes);
        /** @overload */
        GLORY_MONO_API MonoScript(MonoScript&&) noexcept = default;
        /** @brief Move assignment operator */
        GLORY_MONO_API MonoScript& operator=(MonoScript&&) noexcept = default;
        GLORY_MONO_API MonoScript(std::vector<char>&& data);
        GLORY_MONO_API virtual ~MonoScript();

        GLORY_MONO_API void Serialize(Utils::BinaryStream & container) const override {};
        GLORY_MONO_API void Deserialize(Utils::BinaryStream& container) override {};

        GLORY_MONO_API bool HasClass(std::string_view name) const;
        GLORY_MONO_API std::string_view ClassName(size_t index) const;
        GLORY_MONO_API size_t ClassCount() const;

    private:
        virtual void References(IEngine*, std::vector<UUID>&) const override {}

    private:
        friend class MonoScriptLoader;
        std::vector<std::string> m_Classes;
        std::vector<ScriptProperty> m_ScriptProperties;
        std::vector<char> m_DefaultValues;
    };
}
