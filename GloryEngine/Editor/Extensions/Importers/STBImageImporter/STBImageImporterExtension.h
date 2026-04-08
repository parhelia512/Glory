#pragma once
#include <BaseEditorExtension.h>

#ifdef GLORY_STB_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_STB_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_STB_EXTENSION_API __declspec(dllimport)
#endif

EXTENSION_H(GLORY_STB_EXTENSION_API)

namespace Glory::Editor
{
    class STBImageImporterExtension : public BaseEditorExtension
    {
    public:
        STBImageImporterExtension();
        virtual ~STBImageImporterExtension();

    private:
        virtual void Initialize() override;
    };
}
