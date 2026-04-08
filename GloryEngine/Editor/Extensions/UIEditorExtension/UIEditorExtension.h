#pragma once
#include "ui_editor_visibility.h"

#include <BaseEditorExtension.h>

#include <Reflection.h>

EXTENSION_H(GLORY_UI_EXTENSION_API)

namespace Glory::Editor
{
    struct UIElementType
    {
        REFLECTABLE(UIElementType,
            (bool)(m_NewEntity),
            (uint64_t)(m_EntityID)
        );
    };

    class UIEditorExtension : public BaseEditorExtension
    {
    public:
        UIEditorExtension();
        virtual ~UIEditorExtension();

    private:
        virtual void Initialize() override;
    };
}
