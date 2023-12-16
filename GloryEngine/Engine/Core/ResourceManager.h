#pragma once
#include "Resource.h"

#include <vector>
#include <algorithm>

namespace Glory
{
	class BaseResourceManager
	{
	public:
		virtual uint32_t Type() const = 0;
		virtual void Add(Resource* pResource) = 0;

		size_t Index(UUID uuid) const;
		bool IsLoaded(UUID uuid) const;

	protected:
		std::vector<UUID> m_UUIDs;
	};

	template<class T>
	class ResourceManager : public BaseResourceManager
	{
	public:
		virtual uint32_t Type() const override
		{
			return ResourceType::GetHash<T>();
		}

		virtual void Add(Resource* pResource) override
		{
			T* pTResource = static_cast<T*>(pResource);
			Add(std::move(*pTResource));
			delete pResource;
		}

		void Add(T&& resource)
		{
			size_t index = m_Resources.size();
			m_Resources.push_back(std::move(resource));
			m_UUIDs.push_back(m_Resources[index].GetUUID());
		}

		T* Get(UUID uuid)
		{
			const size_t index = Index(uuid);
			return index == UINT64_MAX ? nullptr : &m_Resources[index];
		}

	private:
		std::vector<T> m_Resources;
	};
}
