#pragma once
#include "IConsole.h"

#include <engine_visibility.h>

#include <string>
#include <vector>
#include <thread>

namespace Glory
{
	class Console;
	class BaseConsoleCommand;

	class WindowsDebugConsole : public IConsole
	{
	public:
		GLORY_ENGINE_API WindowsDebugConsole(Console* pConsole);
		GLORY_ENGINE_API virtual ~WindowsDebugConsole();

		GLORY_ENGINE_API bool Running();
		GLORY_ENGINE_API void WaitForInput();

	private:
		void Initialize() override;
		void OnConsoleClose() override;

		void SetNextColor(const glm::vec4& color) override;
		void ResetNextColor() override;
		void Write(const std::string& line) override;

	private:
		friend class Console;
		friend class Debug;
		Console* m_pConsole;
		std::thread m_pConsoleThread;
		bool m_Running;
	};
}