#pragma once
#include "Commands.h"
#include "IConsole.h"

#include <engine_visibility.h>

#include <UUID.h>

#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <filesystem>
#include <map>
#include <vector>

namespace Glory
{
	class IEngine;

	/** @brief Console variable data */
	struct CVar
	{
		std::string m_Name;
		std::string m_Description;
		float m_Value;

		/** @brief CVar flags
		 * @todo: These dont do anything yet */
		enum Flags
		{
			/** @brief No flags */
			None = 0,
			/** @brief Write out value on save */
			Save = 1 << 0,
			/** @brief Changing this value is only allowed when cheats are enabled */
			Cheat = 1 << 1,
			/** @brief Enables cheats */
			CheatsOn = 1 << 2,
			/** @brief Server variable */
			Server = 1 << 3,
			/** @brief Requires authentication */
			RequiresAuth = 1 << 4
		};
		Flags m_Flags;
	};

	/** @brief Console */
	class Console
	{
	public:
		/** @brief Constructor */
		GLORY_ENGINE_API Console();
		/** @brief Destructor */
		GLORY_ENGINE_API virtual ~Console();

		/** @brief Initialize the console with built-in commands */
		GLORY_ENGINE_API void Initialize();
		/** @brief Cleanup the console */
		GLORY_ENGINE_API void Cleanup();

		GLORY_ENGINE_API void RegisterCommand(BaseConsoleCommand* pCommand);
		GLORY_ENGINE_API void RegisterCVar(CVar&& var);
		GLORY_ENGINE_API void QueueCommand(const std::string& command);
		GLORY_ENGINE_API void ExecuteCommand(const std::string& command, bool addToHistory = true);
		GLORY_ENGINE_API void WriteLine(const std::string& line, bool addTimestamp = true);
		GLORY_ENGINE_API void ForEachCommandInHistory(std::function<void(const std::string&)> callback);

		GLORY_ENGINE_API void SetNextColor(const glm::vec4& color);
		GLORY_ENGINE_API void Update();

		inline void RegisterConsole(IConsole* pConsole)
		{
			m_pConsoles.push_back(pConsole);
			pConsole->Initialize();
		}

		inline void RemoveConsole(IConsole* pConsole)
		{
			auto it = std::find(m_pConsoles.begin(), m_pConsoles.end(), pConsole);
			if (it == m_pConsoles.end()) return;
			m_pConsoles.erase(it);
		}

		/** @brief Set the engine this console runs in,
		 * only required for writing configs to pref path
		 * @param pEngine Engine instance
		 */
		GLORY_ENGINE_API void SetEngine(IEngine* pEngine);

		/** @brief Number of lines currently logged in the console */
		GLORY_ENGINE_API size_t LineCount() const;
		/** @brief Get a specific line at an index
		 * @param index Index of the line
		 */
		GLORY_ENGINE_API std::string_view Line(size_t index) const;
		/** @brief Get the color of a line
		 * @param index Index of the line
		 */
		GLORY_ENGINE_API const glm::vec4& LineColor(size_t index) const;

		/** @biref Get the size of the command history */
		GLORY_ENGINE_API size_t HistoryCount() const;
		/** @biref Get a command in the history at a certain index
		 * @param rewwindIndex How much to rewind on the history
		 */
		GLORY_ENGINE_API const std::string& History(size_t rewwindIndex) const;

		/** @brief Find closest command or CVar based on an input and output all possible commands to console
		 * @param input The input to search for
		 * @returns The closest string to all possible commands and cvars
		 */
		GLORY_ENGINE_API std::string AutoComplete(const std::string_view input);

		/** @brief Write all CVars with the @ref CVar::Save flag to a file
		 * @param path Path to write the config to
		 * @returns @cpp true @ce if writing was successfull, @cpp false @ce otherwise
		 */
		GLORY_ENGINE_API bool WriteConfig(std::filesystem::path path);
		/** @brief Read a file and execute each line as a command
		 * @param path Path to the file to read
		 * @returns @cpp true @ce if reading was successfull, @cpp false @ce otherwise
		 */
		GLORY_ENGINE_API bool Exec(const std::filesystem::path& path);
		/** @brief Register a changehandler for when a CVar changes its value
		 * @param cvarName Name of the CVar to listen to
		 * @param handler Callback to the change handler
		 */
		GLORY_ENGINE_API void RegisterCVarChangeHandler(std::string&& cvarName, std::function<void(const CVar*)> handler);

	private:
		bool PrintHistory();
		bool ListVars();

		void AddCommandToHistory(const std::string& command);
		void AddLineToConsole(const std::string& line);

		void SeperateArguments(const std::string& input, std::string& command, std::vector<std::string>& args);
		BaseConsoleCommand* GetCommand(const std::string& command);

		std::string TimeStamp();

		void ExecuteCVarCommand(CVar& cvar, std::vector<std::string>& args);
		CVar* FindCVar(std::string_view name);

	private:
		friend class IEngine;
		friend class Debug;
		friend class GloryContext;
		std::vector<BaseConsoleCommand*> m_pCommands;
		std::vector<IConsole*> m_pConsoles;

		std::mutex m_Lock;

		std::vector<std::string> m_CommandHistory;
		std::vector<std::string> m_ConsoleLines;
		std::vector<glm::vec4> m_ConsoleLineColors;
		glm::vec4 m_CurrentColor;
		std::queue<std::string> m_CommandQueue;
		std::vector<CVar> m_CVars;

		std::map<std::string, std::vector<std::function<void(const CVar*)>>> m_ChangeHandlers;
		IEngine* m_pEngine;
	};
}