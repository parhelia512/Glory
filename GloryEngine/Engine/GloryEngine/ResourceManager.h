#pragma once
#include "Resource.h"
#include "IResourceManager.h"

#include "ResourceType.h"

#include <type_traits>
#include <memory>

namespace Glory
{
	class Resources;

	template<typename R>
	concept ResourceCompatible = std::is_class_v<R> && std::is_base_of<Resource, R>::value && std::movable<R> && requires {
		R();
	};

	template<typename R>
	requires ResourceCompatible<R>
	class ResourceManager : public IResourceManager
	{
	public:
		ResourceManager(Resources* pResources, size_t capacity=32):
			m_pResources(pResources), m_Capacity(capacity), m_Size(0), m_Resources(new R[capacity]) { }
		virtual ~ResourceManager() = default;

		inline virtual uint32_t Type() const override { return TypeHash; };
		inline virtual Resource* Add(Resource* pResource) override
		{
			R* pTypedResource = static_cast<R*>(pResource);
			R& resource = Add(std::move(*pTypedResource));
			delete pResource;
			return static_cast<Resource*>(&resource);
		}

		inline virtual Resource* Get(UUID id) override
		{
			auto iter = m_IDToResourceIndex.find(id);
			if (iter == m_IDToResourceIndex.end()) return nullptr;
			return static_cast<Resource*>(&m_Resources[iter->second]);
		}

		virtual void Remove(UUID id) override
		{
			auto iter = m_IDToResourceIndex.find(id);
			if (iter == m_IDToResourceIndex.end()) return;
			const size_t index = iter->second;
			m_IDToResourceIndex.erase(iter);
			--m_Size;

			for (size_t i = index; i < m_Size; ++i)
				std::swap(m_Resources[i], m_Resources[i + 1]);

			for (auto indexIter = m_IDToResourceIndex.begin(); indexIter != m_IDToResourceIndex.end(); ++indexIter)
			{
				if (indexIter->second < index) continue;
				--indexIter->second;
			}
		}

		inline R& Add(R&& resource)
		{
			if (m_Size == m_Capacity)
				ReserveResources(m_Capacity + std::max(size_t(m_Capacity/2), 0ull));
			const size_t index = m_Size;
			++m_Size;

			m_Resources[index] = std::move(resource);
			m_IDToResourceIndex.emplace(m_Resources[index].GetUUID(), index);
			return m_Resources[index];
		}

		inline static uint32_t TypeHash = ResourceTypes::GetHash<R>();

	private:
		void ReserveResources(size_t newCapacity)
		{
			if (newCapacity <= m_Capacity) return;
			R* newResource = new R[newCapacity];
			for (size_t i = 0; i < m_Size; ++i)
				newResource[i] = std::move(m_Resources[i]);
			m_Resources.reset(newResource);
			m_Capacity = newCapacity;
		}

	private:
		Resources* m_pResources;
		size_t m_Capacity;
		size_t m_Size;
		std::unique_ptr<R[]> m_Resources;
		std::map<UUID, size_t> m_IDToResourceIndex;
	};
}