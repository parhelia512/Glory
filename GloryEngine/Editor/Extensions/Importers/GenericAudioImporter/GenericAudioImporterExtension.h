#pragma once
#include <BaseEditorExtension.h>

#ifdef GLORY_AUDIO_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_AUDIO_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_AUDIO_EXTENSION_API __declspec(dllimport)
#endif

EXTENSION_H(GLORY_AUDIO_EXTENSION_API)

namespace Glory::Editor
{
    class GenericAudioImporterExtension : public BaseEditorExtension
    {
    public:
        GenericAudioImporterExtension();
        virtual ~GenericAudioImporterExtension();

    private:
        virtual void Initialize() override;
    };
}
