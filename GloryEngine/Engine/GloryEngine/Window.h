#pragma once
#include "Input.h"

#include <engine_visibility.h>

#include <Object.h>

#include <string>
#include <vector>

namespace std::filesystem
{
	class path;
}

namespace Glory
{
	class IWindowInputOverrideHandler;
	class WindowModule;
	struct WindowCreateInfo;

	class Window : public Object
	{
	public:
		GLORY_ENGINE_API virtual void GetVulkanRequiredExtensions(std::vector<const char*>& extensions);
		GLORY_ENGINE_API virtual void GetVulkanSurface(void* instance, void* surface);
		GLORY_ENGINE_API virtual void GetDrawableSize(int* width, int* height);
		GLORY_ENGINE_API virtual void GetWindowSize(int* width, int* height);
		virtual void GetWindowPosition(int* x, int* y) = 0;
		virtual void SetupForOpenGL() = 0;
		virtual void CleanupOpenGL() = 0;
		virtual void GLSwapWindow() = 0;
		virtual void MakeGLContextCurrent() = 0;
		virtual void Resize(int width, int height) = 0;
		virtual void GetPosition(int* x, int* y) = 0;
		virtual void SetPosition(int x, int y) = 0;
		virtual void SetCursorPosition(int x, int y) = 0;
		virtual void SetFullscreen(bool fullscreen, bool borderless) = 0;
		virtual void Maximize() = 0;
		virtual void SetGLSwapInterval(int interval) = 0;

		virtual void SetWindowTitle(const char* title) = 0;

		virtual void GetWindowBordersSize(int* top, int* left, int* bottom, int* right) = 0;

		virtual void SetSplashScreen(const char* data, size_t size) = 0;
		virtual void SetSplashScreen(const std::filesystem::path& path) = 0;
		virtual std::string GetClipboardText() = 0;
		virtual void StartTextInput() = 0;
		virtual void StopTextInput() = 0;

		GLORY_ENGINE_API void ShowCursor(bool shown);
		GLORY_ENGINE_API void ForceShowCursor(bool show);
		GLORY_ENGINE_API void ForceUnlockCursor(bool unlock);
		GLORY_ENGINE_API bool IsCursorShown() const;
		GLORY_ENGINE_API void GrabInput(bool grab);
		GLORY_ENGINE_API bool IsGrabInput();
		GLORY_ENGINE_API void ForceUngrabInput(bool ungrab);
		GLORY_ENGINE_API bool HasFocus() const;
		GLORY_ENGINE_API bool IsShown() const;

		GLORY_ENGINE_API void AddInputOverrideHandler(IWindowInputOverrideHandler* handler);
		GLORY_ENGINE_API void RemoveInputOverrideHandler(IWindowInputOverrideHandler* handler);

	protected:
		GLORY_ENGINE_API Window(const WindowCreateInfo& createInfo, WindowModule* pWindowManager);
		GLORY_ENGINE_API virtual ~Window();

		virtual void Open() = 0;
		virtual void Close() = 0;
		virtual void PollEvents() = 0;
		virtual void UpdateCursorShow() = 0;
		virtual void UpdateGrabInput() = 0;

		GLORY_ENGINE_API bool ForwardInputEvent(InputEvent& input);
		GLORY_ENGINE_API void ForwardCursorEvent(CursorEvent& input);
		GLORY_ENGINE_API bool ForwardTextEvent(TextEvent& text);

	protected:
		std::string m_WindowName;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_WindowFlags;
		WindowModule* m_pWindowManager;
		bool m_ShowCursor;
		bool m_ForceShowCursor;
		bool m_ForceUnlockCursor;
		bool m_GrabInput;
		bool m_ForceUngrabInput;

		bool m_HasFocus;
		bool m_IsShown;

		bool m_Fullscreen;
		bool m_Maximized;

		std::vector<IWindowInputOverrideHandler*> m_pInputOverrideHandlers;

	private:
		friend class WindowModule;

	private:
		Window(const Window& y) = delete;
		Window operator=(const Window& y) = delete;
	};
}