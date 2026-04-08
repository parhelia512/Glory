#pragma once
#include "EditorWindowImpl.h"
#include <EditorCreateInfo.h>

#ifdef GLORY_EDITORSDL_EXPORTS
// BUILD LIB
#define GLORY_EDITORSDL_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_EDITORSDL_API __declspec(dllimport)
#endif

namespace Glory::Editor
{
	class EditorApplication;

	extern "C" GLORY_EDITORSDL_API void LoadBackend(EditorCreateInfo& editorCreateInfo);

	class EditorSDLWindowImpl : public EditorWindowImpl
	{
	public:
		EditorSDLWindowImpl();
		virtual ~EditorSDLWindowImpl();

		virtual void SetContext(ImGuiContext* pImguiConext) override;

	protected:
		virtual void Shutdown() override;

		virtual void SetupForOpenGL() override;
		virtual void SetupForVulkan() override;
		virtual void SetupForD3D() override;
		virtual void SetupForMetal() override;

		virtual bool PollEvents() override;
		virtual void NewFrame() override;

	private:
		bool m_AltIsDown;
	};
}
