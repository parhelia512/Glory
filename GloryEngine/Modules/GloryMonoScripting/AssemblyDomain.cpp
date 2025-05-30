#include "AssemblyDomain.h"
#include "Assembly.h"

#include "ScriptExtensions.h"

#include <mono/metadata/exception.h>

namespace Glory
{
	AssemblyDomain::AssemblyDomain(const std::string& name, MonoDomain* pDomain)
		: m_Name(name), m_pMonoDomain(pDomain) {}

	AssemblyDomain::~AssemblyDomain()
	{
		m_pMonoDomain = nullptr;
		m_Assemblies.clear();
	}

	void AssemblyDomain::LoadLib(const ScriptingLib& lib)
	{
		const std::string& name = lib.LibraryName();
		const void* data = lib.Data();
		IMonoLibManager* pLibManager = data ? (IMonoLibManager*)data : nullptr;
		m_Assemblies.emplace(name, Assembly{ this });
		m_Assemblies.at(name).Load(lib, pLibManager);
	}

	void AssemblyDomain::Reload(const std::string& name)
	{
		auto& itor = m_Assemblies.find(name);
		if (itor == m_Assemblies.end()) return;
		Assembly& assembly = itor->second;
		assembly.Unload();
	}

	void AssemblyDomain::Unload(bool isReloading)
	{
		for (auto it = m_Assemblies.begin(); it != m_Assemblies.end(); it++)
		{
			it->second.Unload(isReloading);
		}
		m_Assemblies.clear();
	}

	Assembly* AssemblyDomain::GetAssembly(const std::string& name)
	{
		if (m_Assemblies.find(name) == m_Assemblies.end()) return nullptr;
		return &m_Assemblies.at(name);
	}

	MonoObject* AssemblyDomain::InvokeMethod(MonoMethod* pMethod, MonoObject* pObject, void** args)
	{
		MonoObject* pExceptionObject = nullptr;
		MonoObject* result = mono_runtime_invoke(pMethod, pObject, args, &pExceptionObject);
		if (pExceptionObject)
		{
			mono_print_unhandled_exception(pExceptionObject);
			return nullptr;
		}
		return result;
	}

	size_t AssemblyDomain::AssemblyCount()
	{
		return m_Assemblies.size();
	}

	void AssemblyDomain::ForEachAssembly(std::function<void(Assembly*)> callback)
	{
		for (auto it : m_Assemblies)
		{
			callback(&it.second);
		}
	}

	bool AssemblyDomain::SetCurrentDomain(bool force)
	{
		return mono_domain_set(m_pMonoDomain, force);
	}

	void AssemblyDomain::Initialize()
	{
		for (auto& assembly : m_Assemblies)
		{
			assembly.second.CollectTypes();
		}

		for (auto& assembly : m_Assemblies)
		{
			assembly.second.Initialize();
		}
	}
}
