#pragma once
#include <GloryEditor.h>
#include <Module.h>
#include <GloryContext.h>
#include <BaseEditorExtension.h>

struct ImGuiContext;

extern "C" GLORY_EDITOR_EXTENSION_API Glory::Editor::BaseEditorExtension* LoadExtension();
extern "C" GLORY_EDITOR_EXTENSION_API void SetContext(Glory::GloryContext* pContext, ImGuiContext* pImGUIContext);

namespace Glory::Editor
{
	class ASSIMPImporterExtension : public BaseEditorExtension
	{
	private:
		virtual void RegisterEditors() override;
	};
}