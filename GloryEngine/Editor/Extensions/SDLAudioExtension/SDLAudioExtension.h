#pragma once
#include <BaseEditorExtension.h>
#include <IPlayModeHandler.h>

#ifdef GLORY_SDL_AUDIO_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_SDL_AUDIO_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_SDL_AUDIO_EXTENSION_API __declspec(dllimport)
#endif

EXTENSION_H(GLORY_SDL_AUDIO_EXTENSION_API)

namespace Glory
{
    class Module;
}

namespace Glory::Editor
{
    class SDLAudioExtension : public BaseEditorExtension, public IPlayModeHandler
    {
    public:
        virtual const char* ModuleName() override { return "SDL Audio"; };
        virtual void HandleBeforeStart(Module* pModule) override {};
        virtual void HandleStart(Module* pModule) override {};
        virtual void HandleStop(Module* pModule) override;
        virtual void HandleUpdate(Module* pModule) override {};

        SDLAudioExtension();
        virtual ~SDLAudioExtension();

    private:
        virtual void Initialize() override;
    };
}
