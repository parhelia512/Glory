#pragma once
#include <ModuleMetaData.h>
#include <Version.h>

namespace Glory::EditorLauncher
{
	class ModuleManager
	{
	public:
		static void LoadModules(const std::filesystem::path& editorPath);
		static void Clear();

		static void GetModulesOfType(ModuleType moduleType, std::vector<ModuleMetaData*>& result);
		static bool FindModuleIndex(std::string_view name, ModuleType moduleType, int& index);

	private:
		static void AddModuleMeta(const ModuleMetaData& metaData);

	private:
		ModuleManager();
		virtual ~ModuleManager();

	private:
		static std::vector<ModuleMetaData> m_InstalledModules;
		static std::map<ModuleType, std::vector<size_t>> m_ModulesByTypes;
	};
}
