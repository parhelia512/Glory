#pragma once
#include <Module.h>

#include <engine_visibility.h>

#include <vector>

namespace Glory
{
	class Window;
	struct WindowCreateInfo;

    class WindowModule : public Module
    {
	public:
		GLORY_ENGINE_API WindowModule();
		GLORY_ENGINE_API virtual ~WindowModule();

		GLORY_ENGINE_API Window* GetMainWindow();

		GLORY_ENGINE_API virtual const std::type_info& GetModuleType() override;

		virtual void GetCurrentScreenResolution(uint32_t& width, uint32_t& height) = 0;

		virtual std::filesystem::path GetPrefPath() const = 0;

	public: // Module functions
		GLORY_ENGINE_API Window* CreateNewWindow(WindowCreateInfo& createInfo);
		GLORY_ENGINE_API virtual void OpenMessageBox(const std::string& message);

		GLORY_ENGINE_API virtual void PollEvents();

	protected: // Internal functions
		virtual Window* CreateWindow_Internal(const WindowCreateInfo& createInfo) = 0;

	protected:
		virtual void OnInitialize() = 0;
		virtual void OnCleanup() = 0;

	private:
		GLORY_ENGINE_API virtual void Initialize() override;
		GLORY_ENGINE_API virtual void Cleanup() override;

	private: // Memory stuff
		std::vector<Window*> m_pWindows;
		Window* m_pMainWindow;
    };
}
