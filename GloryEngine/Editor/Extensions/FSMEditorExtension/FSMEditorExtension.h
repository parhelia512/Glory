#pragma once
#include <BaseEditorExtension.h>
#include <IPlayModeHandler.h>

#ifdef GLORY_FSM_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_FSM_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_FSM_EXTENSION_API __declspec(dllimport)
#endif

EXTENSION_H(GLORY_FSM_EXTENSION_API)

namespace Glory
{
    class Module;
}

namespace Glory::Editor
{
    class FSMEditorExtension : public BaseEditorExtension, IPlayModeHandler
    {
    public:
        FSMEditorExtension();
        virtual ~FSMEditorExtension();

    private:
        virtual void Initialize() override;

        virtual const char* ModuleName() override;
        virtual void HandleBeforeStart(Module* pModule) override;
        virtual void HandleStart(Module* pModule) override;
        virtual void HandleStop(Module* pModule) override;
        virtual void HandleUpdate(Module* pModule) override;
    };
}
