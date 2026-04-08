#pragma once
#include "mono_visibility.h"

#include <string>

namespace Glory
{
	class ScriptExtension
	{
	public:
		GLORY_MONO_API ScriptExtension();
		GLORY_MONO_API virtual ~ScriptExtension();

	private:
		friend class ScriptingExtender;
	};

	class ScriptingLib : public ScriptExtension
	{
	public:
		GLORY_MONO_API ScriptingLib(const std::string& libraryName, const std::string& location, bool reloadable, void* data = nullptr);
		GLORY_MONO_API virtual ~ScriptingLib();

		GLORY_MONO_API const std::string& LibraryName() const;
		GLORY_MONO_API const std::string& Location() const;
		GLORY_MONO_API bool Reloadable() const;
		GLORY_MONO_API const void* Data() const;

	private:
		const std::string m_LibraryName;
		const std::string m_Location;
		const bool m_Reloadable;
		const void* m_Data;
	};

	class InternalCall : public ScriptExtension
	{
	public:
		GLORY_MONO_API InternalCall(const std::string& scriptMethod, const void* pRedirectMethod);

		GLORY_MONO_API const std::string& ScriptMethod() const;
		GLORY_MONO_API const void* RedirectMethod() const;

	private:
		friend class ScriptingExtender;
		std::string m_ScriptMethod;
		const void* m_pRedirectMethod;
	};
}
