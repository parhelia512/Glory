#include "ModuleSettings.h"

namespace Glory
{
	ModuleSettings::ModuleSettings()
		: m_SettingsNode(0) {}

	ModuleSettings::ModuleSettings(YAML::Node& settingsNode)
		: m_SettingsNode(settingsNode), m_GroupNames({ "General" }), m_Groups({ {"General", std::vector<std::string>()}}) {}

	ModuleSettings::~ModuleSettings() {}

	void ModuleSettings::PushGroup(const std::string& name)
	{
		m_GroupNames.push_back(name);
	}

	std::vector<std::string>::iterator ModuleSettings::GroupsBegin()
	{
		return m_GroupNames.begin();
	}

	std::vector<std::string>::iterator ModuleSettings::GroupsEnd()
	{
		return m_GroupNames.end();
	}

	std::vector<std::string>::iterator ModuleSettings::Begin(const std::string& group)
	{
		return m_Groups.at(group).begin();
	}

	std::vector<std::string>::iterator ModuleSettings::End(const std::string& group)
	{
		return m_Groups.at(group).end();
	}

	const uint32_t ModuleSettings::Type(const std::string& name) const
	{
		if (m_Types.find(name) == m_Types.end()) return 0;
		return m_Types.at(name);
	}

	YAML::Node& ModuleSettings::Node()
	{
		return m_SettingsNode;
	}

	void ModuleSettings::RegisterValue(const std::string& name, uint32_t type)
	{
		m_ValueNames.push_back(name);
		m_Types.emplace(m_ValueNames.back(), type);
		const std::string& group = m_GroupNames.back();
		if (m_Groups.find(group) == m_Groups.end())
			m_Groups.emplace(group, std::vector<std::string>());
		m_Groups[group].push_back(m_ValueNames.back());
	}
}