#pragma once
#include "IConsole.h"

#include <engine_visibility.h>

namespace Glory
{
	class Console;

	class Logs : public IConsole
	{
	public:
		GLORY_ENGINE_API Logs() {};

	private:
		void Initialize() override;
		void OnConsoleClose() override {};
		void SetNextColor(const glm::vec4& color) override {};
		void ResetNextColor() override {};
		void Write(const std::string& line) override;

	private:
		friend class Console;
	};
}
