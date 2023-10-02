#include "ASSIMPImporterExtension.h"
#include "ASSIMPImporter.h"

#include <imgui.h>
#include <Importer.h>

Glory::Editor::BaseEditorExtension* LoadExtension()
{
	return new Glory::Editor::ASSIMPImporterExtension();
}

void SetContext(Glory::GloryContext* pContext, ImGuiContext* pImGUIContext)
{
	Glory::GloryContext::SetContext(pContext);
	ImGui::SetCurrentContext(pImGUIContext);
}

namespace Glory::Editor
{
	void ASSIMPImporterExtension::RegisterEditors()
	{
		Importer::Register<ASSIMPImporter>();
	}
}
