#pragma once
#include "Resource.h"

#include <engine_visibility.h>

#include <string>
#include <string_view>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include <functional>

namespace Glory
{
	class Resource;

	struct BasicTypeData
	{
		GLORY_ENGINE_API BasicTypeData(const std::string& name, uint32_t typeHash, size_t size);

		const std::string m_Name;
		uint32_t m_TypeHash;
		size_t m_Size;
	};

	class ResourceFactory
	{
	public:
		virtual Resource* Create() const = 0;
	};

	template<typename R>
	class ResourceFactoryTemplate : public ResourceFactory
	{
	public:
		ResourceFactoryTemplate(std::function<void(R*)>&& handler):
			m_CreationHandler(std::move(handler)) {}

		inline virtual Resource* Create() const override
		{
			R* pNewResource = new R();
			if (m_CreationHandler) m_CreationHandler(pNewResource);
			return (Resource*)pNewResource;
		}

	private:
		std::function<void(R*)> m_CreationHandler = NULL;
	};

	class ResourceType
	{
	public:
		GLORY_ENGINE_API virtual ~ResourceType();
		GLORY_ENGINE_API uint32_t Hash() const;
		GLORY_ENGINE_API const std::string& Extensions() const;
		GLORY_ENGINE_API const std::string& FullName() const;
		GLORY_ENGINE_API const std::string& Name() const;
		GLORY_ENGINE_API Resource* Create() const;

	private:
		ResourceType(uint32_t typeHash, const std::string& extensions, const char* name, ResourceFactory* pFactory);
		const ResourceType operator=(const ResourceType&) = delete;

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
		template<class T>
		inline void RegisterResource(const std::string& extensions, std::function<void(T*)>&& handler=NULL)
		{
			ResourceFactory* pFactory = new ResourceFactoryTemplate<T>(std::move(handler));
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
		inline void RegisterType()
		{
			RegisterType(typeid(T), sizeof(T));
		}

		template<typename T>
		inline static uint32_t GetHash()
		{
			return GetHash(typeid(T));
		}

		template<typename T>
		inline ResourceType* GetResourceType()
		{
			return GetResourceType(typeid(T));
		}

		GLORY_ENGINE_API bool IsResource(uint32_t typeHash);
		GLORY_ENGINE_API ResourceType* RegisterResource(std::type_index type, const std::string& extensions, ResourceFactory* pFactory);
		GLORY_ENGINE_API void RegisterType(const std::type_info& type, size_t size);

		GLORY_ENGINE_API static uint32_t GetHash(std::type_index type);
		GLORY_ENGINE_API ResourceType* GetResourceType(const std::string& extension);
		GLORY_ENGINE_API ResourceType* GetResourceType(std::type_index type);
		GLORY_ENGINE_API ResourceType* GetResourceType(uint32_t hash);

		GLORY_ENGINE_API const BasicTypeData* GetBasicTypeData(uint32_t typeHash);
		GLORY_ENGINE_API const BasicTypeData* GetBasicTypeData(const std::string& name);

		GLORY_ENGINE_API static size_t SubTypeCount(const ResourceType* pResourceType);
		GLORY_ENGINE_API ResourceType* GetSubType(const ResourceType* pResourceType, size_t index);
		GLORY_ENGINE_API static uint32_t GetSubTypeHash(const ResourceType* pResourceType, size_t index);
		GLORY_ENGINE_API size_t GetAllResourceTypesThatHaveSubType(uint32_t hash, std::vector<ResourceType*>& out);

		GLORY_ENGINE_API static bool IsScene(const std::string& ext);

	public:
		ResourceTypes();
		virtual ~ResourceTypes();

	private:
		void ReadExtensions(size_t index, const std::string& extensions);

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
