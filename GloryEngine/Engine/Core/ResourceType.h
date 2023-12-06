#pragma once
#include "UUID.h"

#include <string>
#include <string_view>
#include <typeindex>
#include <vector>
#include <unordered_map>

namespace Glory
{
	class Resource;

	struct BasicTypeData
	{
		BasicTypeData(const std::string& name, uint32_t typeHash, size_t size);

		const std::string m_Name;
		uint32_t m_TypeHash;
		size_t m_Size;
	};

	class ResourceFactory
	{
	public:
		virtual Resource* Create() const = 0;
		virtual Resource* Create(const UUID uuid, const std::string& name) const = 0;
	};

	template<typename R>
	class ResourceFactoryTemplate : public ResourceFactory
	{
	public:
		virtual Resource* Create() const override
		{
			return (Resource*)new R();
		}

		virtual Resource* Create(const UUID uuid, const std::string& name) const override
		{
			return (Resource*)new R(uuid, name);
		}
	};

	class ResourceType
	{
	public:
		template<class T>
		static void RegisterResource(const std::string& extensions = "")
		{
			ResourceFactory* pFactory = new ResourceFactoryTemplate<T>();
			ResourceType* pResourceType = RegisterResource(typeid(T), extensions, pFactory);
			T t = T();
			for (size_t i = 0; i < t.TypeCount(); i++)
			{
				std::type_index type = typeid(Object);
				if (!t.GetType(i, type)) continue;
				uint32_t hash = GetHash(type);
				pResourceType->m_SubTypes.push_back(hash);
			}
		}

		template<typename T>
		static void RegisterType()
		{
			RegisterType(typeid(T), sizeof(T));
		}

		template<typename T>
		static uint32_t GetHash()
		{
			return GetHash(typeid(T));
		}

		template<typename T>
		static ResourceType* GetResourceType()
		{
			return GetResourceType(typeid(T));
		}

		static bool IsResource(uint32_t typeHash);
		static ResourceType* RegisterResource(std::type_index type, const std::string& extensions, ResourceFactory* pFactory);
		static void RegisterType(const std::type_info& type, size_t size);
		static uint32_t GetHash(std::type_index type);
		static ResourceType* GetResourceType(const std::string& extension);
		static ResourceType* GetResourceType(std::type_index type);
		static ResourceType* GetResourceType(uint32_t hash);
		static const BasicTypeData* GetBasicTypeData(uint32_t typeHash);
		static const BasicTypeData* GetBasicTypeData(const std::string& name);

		static size_t SubTypeCount(const ResourceType* pResourceType);
		static ResourceType* GetSubType(const ResourceType* pResourceType, size_t index);
		static uint32_t GetSubTypeHash(const ResourceType* pResourceType, size_t index);
		static size_t GetAllResourceTypesThatHaveSubType(uint32_t hash, std::vector<ResourceType*>& out);

		static bool IsScene(const std::string& ext);

	public:
		virtual ~ResourceType();
		uint32_t Hash() const;
		const std::string& Extensions() const;
		const std::string& FullName() const;
		const std::string& Name() const;
		Resource* Create() const;
		Resource* Create(const UUID uuid, const std::string& name) const;

	private:
		ResourceType(uint32_t typeHash, const std::string& extensions, const char* name, ResourceFactory* pFactory);
		const ResourceType operator=(const ResourceType&) = delete;

	private:
		static void ReadExtensions(size_t index, const std::string& extensions);

	private:
		friend class ResourceTypes;
		const uint32_t m_TypeHash;
		const std::string m_Extensions;
		const std::string m_FullName;
		std::string m_Name;
		std::vector<uint32_t> m_SubTypes;
		ResourceFactory* m_pFactory;
	};

	class ResourceTypes
	{
	public:
		ResourceTypes();
		virtual ~ResourceTypes();

	private:
		friend class ResourceType;
		std::vector<ResourceType> m_ResourceTypes;
		std::unordered_map<std::string, size_t> m_ExtensionToType;
		std::unordered_map<uint32_t, size_t> m_HashToType;

		std::vector<BasicTypeData> m_BasicTypes;
		std::unordered_map<uint32_t, size_t> m_HashToBasicType;
		std::unordered_map<std::string, size_t> m_NameToBasicType;
	};
}
