#pragma once
#include <BaseEditorExtension.h>

#ifdef GLORY_FREETYPE_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_FREETYPE_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_FREETYPE_EXTENSION_API __declspec(dllimport)
#endif

EXTENSION_H(GLORY_FREETYPE_EXTENSION_API)

namespace Glory::Editor
{
    class FreeTypeFontImporter : public BaseEditorExtension
    {
    public:
        FreeTypeFontImporter();
        virtual ~FreeTypeFontImporter();

    private:
        virtual void Initialize() override;
    };
}
