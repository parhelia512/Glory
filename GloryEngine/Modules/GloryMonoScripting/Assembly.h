#pragma once
#include "mono_visibility.h"

#include <mono/jit/jit.h>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <filesystem>

namespace Glory
{
	class ScriptingLib;

	enum class Visibility
	{
		VISIBILITY_PRIVATE = 1,
		VISIBILITY_PROTECTED_AND_INTERNAL,
		VISIBILITY_INTERNAL,
		VISIBILITY_PROTECTED,
		VISIBILITY_PROTECTED_OR_INTERNAL,
		VISIBILITY_PUBLIC,
	};

	struct AssemblyClassField
	{
	public:
		AssemblyClassField();
		AssemblyClassField(MonoClassField* pField);
		virtual ~AssemblyClassField();

	public:
		void SetValue(MonoObject* pObject, void* value) const;
		void GetValue(MonoObject* pObject, void* value) const;

		MonoClassField* ClassField() const;
		MonoType* FieldType() const;
		const char* Name() const;
		const Visibility& FieldVisibility() const;
		const char* TypeName() const;
		const int Size() const;
		const uint32_t TypeHash() const;
		const uint32_t ElementTypeHash() const;
		const bool IsStatic() const;

	private:
		MonoClassField* m_pMonoField;
		MonoType* m_pType;
		uint32_t m_Flags;
		const char* m_Name;
		Visibility m_Visibility;
		const char* m_TypeName;
		int m_Size;
		int m_SizeAllignment;
		uint32_t m_TypeHash;
		uint32_t m_ElementTypeHash;
		bool m_IsStatic;
	};

	struct AssemblyClass
	{
	public:
		AssemblyClass();
		AssemblyClass(const std::string& name, MonoClass* pClass);

		std::string m_Name;
		MonoClass* m_pClass;
		std::map<std::string, MonoMethod*> m_pMethods;

		GLORY_MONO_API MonoMethod* GetMethod(const std::string& name);
		GLORY_MONO_API const AssemblyClassField* GetField(const std::string& name) const;
		GLORY_MONO_API const AssemblyClassField* GetField(const size_t index) const;
		GLORY_MONO_API const size_t NumFields() const;

	private:
		MonoMethod* LoadMethod(const std::string& name);

		void LoadFields();

		void LoadFieldsRecursive(MonoClass* pClass);

		std::vector<AssemblyClassField> m_Fields;
		std::map<std::string, size_t> m_NameToFieldIndex;
	};

	struct AssemblyNamespace
	{
		std::string m_Name;
		std::map<std::string, AssemblyClass> m_Classes;
	};

	class IMonoLibManager;
	class AssemblyDomain;

	class Assembly
	{
	public:
		enum AssemblyState
		{
			AS_NotLoaded,
			AS_Loading,
			AS_Loaded,
		};

	public:
		GLORY_MONO_API Assembly(AssemblyDomain* pDomain);
		virtual GLORY_MONO_API ~Assembly();

		GLORY_MONO_API AssemblyClass* GetClass(const std::string& namespaceName, const std::string& className);
		GLORY_MONO_API bool GetClass(const std::string& namespaceName, const std::string& className, AssemblyClass& c);

		GLORY_MONO_API bool Load(const ScriptingLib& lib, IMonoLibManager* pLibManager);
		GLORY_MONO_API bool Load(MonoImage* monoImage);
		GLORY_MONO_API void Unload(bool isReloading = false);
		GLORY_MONO_API MonoReflectionAssembly* GetReflectionAssembly() const;
		GLORY_MONO_API void Initialize();
		GLORY_MONO_API void CollectTypes();

		GLORY_MONO_API bool IsLoading() const
		{
			return m_State == AssemblyState::AS_Loading;
		}

		GLORY_MONO_API bool IsLoaded() const
		{
			return m_State == AssemblyState::AS_Loaded;
		}

		GLORY_MONO_API AssemblyDomain* GetDomain() const
		{
			return m_pDomain;
		}

		GLORY_MONO_API MonoAssembly* GetMonoAssembly() const
		{
			return m_pAssembly;
		}

		GLORY_MONO_API MonoImage* GetMonoImage()
		{
			return m_pImage;
		}

		GLORY_MONO_API const std::string& Name()
		{
			return m_Name;
		}

		GLORY_MONO_API const std::string& Location()
		{
			return m_Location;
		}

		GLORY_MONO_API IMonoLibManager* LibManager()
		{
			return m_pLibManager;
		}

	private:
		bool LoadAssembly(const std::filesystem::path& assemblyPath);
		bool LoadAssemblyWithImage(const std::filesystem::path& assemblyPath);
		AssemblyClass* LoadClass(AssemblyNamespace* pNamespace, const std::string& className);

	private:
		friend class AssemblyDomain;
		AssemblyDomain* m_pDomain;
		MonoAssembly* m_pAssembly;
		MonoImage* m_pImage;
		std::map<std::string, AssemblyNamespace> m_Namespaces;
		IMonoLibManager* m_pLibManager;

		AssemblyState m_State = AssemblyState::AS_NotLoaded;
		bool m_Locked = false;

		std::string m_Name;
		std::string m_Location;
		bool m_Reloadable;
	};

	class Attributes
	{
	public:
		Attributes();
		~Attributes();

	private:
		std::map<std::string, MonoClass*> m_AttributeClasses;
	};

	GLORY_MONO_API void AddMonoType(std::string name, uint32_t type, uint32_t elementType);
}
