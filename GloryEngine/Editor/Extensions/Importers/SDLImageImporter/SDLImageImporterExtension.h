#pragma once
#include <BaseEditorExtension.h>

#ifdef GLORY_SDL_IMAGE_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_SDL_IMAGE_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_SDL_IMAGE_EXTENSION_API __declspec(dllimport)
#endif

EXTENSION_H(GLORY_SDL_IMAGE_EXTENSION_API)

namespace Glory::Editor
{
    class SDLImageImporterExtension : public BaseEditorExtension
    {
    public:
        SDLImageImporterExtension();
        virtual ~SDLImageImporterExtension();

    private:
        virtual void Initialize() override;
    };
}
