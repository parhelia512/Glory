#include "Console.h"
#include "Commands.h"
#include "Engine.h"
#include "WindowModule.h"
#include "StringUtils.h"

#include <iostream>
#include <iomanip>
#include <fstream>

namespace Glory
{
	void Console::Initialize()
	{
		RegisterCommand(new ConsoleCommand("printhistory", [this]() { return PrintHistory(); }));
		RegisterCommand(new ConsoleCommand("listvars", [this]() { return ListVars(); }));
		RegisterCommand(new ConsoleCommand("writeconfig", [this]() { return WriteConfig(""); }));
		RegisterCommand(new ConsoleCommand1<std::string>("writeconfigto", [this](std::string path) { return WriteConfig(path); }));
		RegisterCommand(new ConsoleCommand1<std::string>("exec", [this](std::string path) { return Exec(path); }));

		m_ConsoleLines.reserve(1000);
		m_CommandHistory.reserve(1000);
	}

	void Console::Cleanup()
	{
		m_pCommands.clear();
		m_pConsoles.clear();
		m_CommandHistory.clear();
		m_ConsoleLines.clear();
		m_ConsoleLineColors.clear();
		m_CVars.clear();
	}

	void Console::Update()
	{
		if (m_CommandQueue.empty()) return;
		const std::string& command = m_CommandQueue.front();
		ExecuteCommand(command);
		m_CommandQueue.pop();
	}

	void Console::SetEngine(Engine* pEngine)
	{
		m_pEngine = pEngine;
	}

	size_t Console::LineCount() const
	{
		return m_ConsoleLines.size();
	}

	std::string_view Console::Line(size_t index) const
	{
		return m_ConsoleLines[index];
	}

	const glm::vec4& Console::LineColor(size_t index) const
	{
		return m_ConsoleLineColors[index];
	}

	size_t Console::HistoryCount() const
	{
		return m_CommandHistory.size();
	}

	const std::string& Console::History(size_t rewindIndex) const
	{
		const size_t index = rewindIndex >= m_CommandHistory.size() ? 0 : m_CommandHistory.size() - 1 - rewindIndex;
		return m_CommandHistory[index];
	}

	std::string Console::AutoComplete(const std::string_view input)
	{
		std::vector<std::string> possibleOptions;
		/* Find commands */
		for (size_t i = 0; i < m_pCommands.size(); ++i)
		{
			if (Utils::CaseInsensitiveSearch(m_pCommands[i]->m_Command, input) == std::string::npos)
				continue;
			possibleOptions.push_back(m_pCommands[i]->m_Command);
		}

		/* Find CVars */
		for (size_t i = 0; i < m_CVars.size(); ++i)
		{
			if (Utils::CaseInsensitiveSearch(m_CVars[i].m_Name, input) == std::string::npos)
				continue;
			possibleOptions.push_back(m_CVars[i].m_Name);
		}

		if (possibleOptions.empty()) return "";
		if (possibleOptions.size() == 1) return possibleOptions[0];

		std::string closest = "";

		/* Find closest string while printing all options */
		for (size_t i = 0; i < possibleOptions[0].size(); ++i)
		{
			bool found = true;
			for (size_t j = 1; j < possibleOptions.size(); ++j)
			{
				if (i == 0) WriteLine(possibleOptions[j], false);
				if (i >= possibleOptions[j].size() || possibleOptions[0][i] != possibleOptions[j][i])
				{
					found = false;
					break;
				}
			}
			if (!found) break;
			closest += possibleOptions[0][i];
		}
		return closest;
	}

	bool Console::WriteConfig(std::filesystem::path path)
	{
		if (path.empty())
		{
			if (!m_pEngine)
			{
				WriteLine("Need a path to write the config to");
				return false;
			}
			WindowModule* pWindows = m_pEngine->GetMainModule<WindowModule>();
			if (!pWindows)
			{
				WriteLine("Need a path to write the config to");
				return false;
			}
			path = pWindows->GetPrefPath();
			path.append("config.cfg");
		}

		std::ofstream stream{ path };
		if (!stream.is_open())
		{
			WriteLine("Failed to write config to: " + path.string());
			return false;
		}

		for (size_t i = 0; i < m_CVars.size(); ++i)
		{
			const CVar& cvar = m_CVars[i];
			if (!(cvar.m_Flags & CVar::Save)) continue;
			stream << cvar.m_Name << " " << cvar.m_Value << std::endl;
		}
		stream.close();
		return true;
	}

	bool Console::Exec(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			WriteLine("Failed to open config at: " + path.string() + " file does not exist!");
			return false;
		}

		std::ifstream stream{ path };
		if (!stream.is_open())
		{
			WriteLine("Failed to read config at: " + path.string());
			return false;
		}

		std::string line = "";
		while (!stream.eof())
		{
			std::getline(stream, line);
			if (line.empty()) continue;
			QueueCommand(line);
		}
		return true;
	}

	bool Console::PrintHistory()
	{
		ForEachCommandInHistory([=](const std::string& command)
		{
			WriteLine(command);
		});
		return true;
	}

	bool Console::ListVars()
	{
		for (const CVar& cvar : m_CVars)
			WriteLine(cvar.m_Name + " = " + std::to_string(cvar.m_Value) + " : " + cvar.m_Description);

		return true;
	}

	void Console::AddCommandToHistory(const std::string& command)
	{
		m_CommandHistory.emplace_back(command);
	}

	void Console::AddLineToConsole(const std::string& line)
	{
		m_ConsoleLines.push_back(line);
		m_ConsoleLineColors.push_back(m_CurrentColor);
	}

	void Console::RegisterCommand(BaseConsoleCommand* pCommand)
	{
		m_pCommands.push_back(pCommand);
	}

	void Console::RegisterCVar(CVar&& var)
	{
		m_CVars.emplace_back(std::move(var));
	}

	void Console::QueueCommand(const std::string& command)
	{
		m_CommandQueue.push(command);
	}

	void Console::ExecuteCommand(const std::string& command, bool addToHistory)
	{
		if (command == "") return;

		if (addToHistory) AddCommandToHistory(command);
		WriteLine(command, false);

		std::string commandText;
		std::vector<std::string> args;
		SeperateArguments(command, commandText, args);
		BaseConsoleCommand* pCommand = GetCommand(commandText);

		if (pCommand != nullptr)
		{
			if (pCommand->RootExecuteCommand(args)) return;
			WriteLine(">>>> Could not execute command", false);
			return;
		}

		CVar* cvar = FindCVar(commandText);
		if (!cvar)
		{
			WriteLine(">>>> Unknown Command", false);
			return;
		}

		ExecuteCVarCommand(*cvar, args);
	}

	void Console::WriteLine(const std::string& line, bool addTimestamp)
	{
		std::unique_lock lock{m_Lock};

		std::string finalLine = line;
		if (addTimestamp) finalLine = TimeStamp() + finalLine;
		AddLineToConsole(finalLine);

		for (size_t i = 0; i < m_pConsoles.size(); i++)
		{
			m_pConsoles[i]->Write(finalLine);
		}
	}

	void Console::ForEachCommandInHistory(std::function<void(const std::string&)> callback)
	{
		for (size_t i = m_CommandHistory.size(); i > 0; --i)
		{
			const std::string& command = m_CommandHistory[i-1];
			callback(command);
		}
	}

	void Console::SetNextColor(const glm::vec4& color)
	{
		m_CurrentColor = color;
		for (size_t i = 0; i < m_pConsoles.size(); i++)
		{
			m_pConsoles[i]->SetNextColor(color);
		}
	}

	void Console::SeperateArguments(const std::string& input, std::string& command, std::vector<std::string>& args)
	{
		command = "";
		args.clear();

		int argNumber = 0;
		std::string currentArg = "";
		for (size_t i = 0; i < input.size(); i++)
		{
			char c = input[i];
			if (c != ' ')
			{
				currentArg.push_back(c);

				if (i == input.size() - 1)
				{
					if (argNumber == 0)
					{
						command = currentArg;
						currentArg = "";
						++argNumber;
						continue;
					}
					args.push_back(currentArg);
					currentArg = "";
					++argNumber;
				}
			}
			else
			{
				if (argNumber == 0)
				{
					command = currentArg;
					currentArg = "";
					++argNumber;
					continue;
				}
				args.push_back(currentArg);
				currentArg = "";
				++argNumber;
			}
		}
	}

	BaseConsoleCommand* Console::GetCommand(const std::string& command)
	{
		auto it = std::find_if(m_pCommands.begin(), m_pCommands.end(), [&](BaseConsoleCommand* pCommand)
		{
			if (pCommand->GetCommand() == command) return true;
			return false;
		});

		if (it == m_pCommands.end()) return nullptr;
		BaseConsoleCommand* pCommand = *it;
		return pCommand;
	}

	std::string Console::TimeStamp()
	{
		auto t = time(nullptr);
		tm tM;
		localtime_s(&tM, &t);
		std::stringstream sstream;
		sstream << "[" << std::put_time(&tM, "%d-%m-%Y %H-%M-%S") << "] ";
		return sstream.str();
	}

	CVar* Console::FindCVar(std::string_view name)
	{
		auto& iter = std::find_if(m_CVars.begin(), m_CVars.end(), [name](const CVar& cvar) { return cvar.m_Name == name; });
		if (iter == m_CVars.end()) return nullptr;
		return &*iter;
	}

	void Console::RegisterCVarChangeHandler(std::string&& cvarName, std::function<void(const CVar*)> handler)
	{
		auto iter = m_ChangeHandlers.find(cvarName);
		if (iter == m_ChangeHandlers.end())
		{
			m_ChangeHandlers.emplace(std::move(cvarName),
				std::vector<std::function<void(const CVar*)>>{ handler });
			return;
		}
		iter->second.emplace_back(handler);
	}

	void Console::ExecuteCVarCommand(CVar& cvar, std::vector<std::string>& args)
	{
		if (args.empty())
		{
			/* Print description and current value */
			std::stringstream stream;
			stream << cvar.m_Name << " = " << cvar.m_Value;
			WriteLine(stream.str());
			WriteLine(cvar.m_Description);
			return;
		}

		/* Change the value */
		if (Parser::Parse<float>(args[0], cvar.m_Value))
		{
			auto& iter = m_ChangeHandlers.find(cvar.m_Name);
			if (iter == m_ChangeHandlers.end()) return;
			for (auto& handler : iter->second)
			{
				handler(&cvar);
			}
			return;
		}
		WriteLine("Error: Value must be a number");
	}

	Console::Console():
		m_CommandHistory(), m_ConsoleLines(),
		m_CurrentColor(1.0f, 1.0f, 1.0f, 1.0f), m_pEngine(nullptr)
	{}

	Console::~Console()
	{
		for (auto pCommand : m_pCommands)
		{
			delete pCommand;
		}
		m_pCommands.clear();

		Parser::Destroy();

		for (size_t i = 0; i < m_pConsoles.size(); i++)
		{
			m_pConsoles[i]->OnConsoleClose();
		}
		m_pConsoles.clear();

		m_CommandHistory.clear();
		m_ConsoleLines.clear();
		m_ConsoleLineColors.clear();
	}
}
