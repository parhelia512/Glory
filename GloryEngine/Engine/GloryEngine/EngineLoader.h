#pragma once
#include "GloryEngine.h"
#include "Window.h"

#include <engine_visibility.h>

#include <Module.h>

#include <string>
#include <filesystem>
#include <windows.h>

namespace Glory
{
	class Console;
	class Debug;

	class EngineLoader
	{
	public:
		GLORY_ENGINE_API EngineLoader(const std::filesystem::path& cfgPath);
		GLORY_ENGINE_API virtual ~EngineLoader();

		GLORY_ENGINE_API GloryEngine LoadEngine(Console* pConsole, Debug* pDebug);
		GLORY_ENGINE_API GloryEngine LoadEngineFromPath(Console* pConsole, Debug* pDebug);
		GLORY_ENGINE_API EngineCreateInfo LoadEngineInfoFromPath(Console* pConsole, Debug* pDebug);
		GLORY_ENGINE_API void Unload();

		GLORY_ENGINE_API const size_t ModuleCount() const;
		GLORY_ENGINE_API const Module* GetModule(size_t index) const;

	private:
		void LoadModules(YAML::Node& modules);
		void LoadModule(std::string& moduleName);
		void ReadModule(Module* pModule);
		void LoadExtras();
		void LoadExtra(const std::string& name, const std::filesystem::path& path, Module* pModule, Module* pRequiredModule);

	private:
		const std::filesystem::path m_CFGPath;
		std::vector<HMODULE> m_DependencyLibs;
		std::vector<HMODULE> m_ModuleLibs;
		std::vector<HMODULE> m_ExtraLibs;
		std::vector<Module*> m_pMainModules;
		std::vector<Module*> m_pAllModules;
		std::vector<std::string> m_LoadedModuleNames;

		EngineCreateInfo m_EngineInfo;
		std::vector<Module*> m_pOptionalModules;
	};
}
