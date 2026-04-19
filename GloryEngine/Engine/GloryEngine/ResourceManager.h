#pragma once
#include "Resource.h"
#include "IResourceManager.h"

#include "ResourceType.h"

#include <type_traits>
#include <memory>

namespace Glory
{
	class Resources;

	/** @brief Constraint for resource compatible types. */
	template<typename R>
	concept ResourceCompatible = std::is_class_v<R> && std::is_base_of<Resource, R>::value && std::movable<R> && requires {
		R();
	};

	/** @brief Resource manager */
	template<typename R>
	requires ResourceCompatible<R>
	class ResourceManager : public IResourceManager
	{
	public:
		/** @brief Constructor
		 * @param pResources Global resources manager instance.
		 * @param capacity Initial capacity of this manager.
		 */
		ResourceManager(Resources* pResources, size_t capacity=32):
			m_pResources(pResources), m_Capacity(capacity), m_Size(0), m_Resources(new R[capacity]) { }
		/** @brief Destructor */
		virtual ~ResourceManager() = default;

		/** @brief Type hash of the resource, initialized using ResourceTypes::GetHash<R>() */
		inline virtual uint32_t Type() const override { return TypeHash; };
		/** @brief Add a resource to the manager.
		 * @param pResource Resource to add.
		 */
		inline virtual Resource* Add(Resource* pResource) override
		{
			R* pTypedResource = static_cast<R*>(pResource);
			R& resource = Add(std::move(*pTypedResource));
			delete pResource;
			return static_cast<Resource*>(&resource);
		}

		/** @brief Find a resource in this manager.
		 * @param id ID of the resource to find.
		 */
		inline virtual Resource* Get(UUID id) override
		{
			auto iter = m_IDToResourceIndex.find(id);
			if (iter == m_IDToResourceIndex.end()) return nullptr;
			return static_cast<Resource*>(&m_Resources[iter->second]);
		}

		/** @brief Find a resource in this manager, but return it directly instead of casting.
		 * @param id ID of the resource to find.
		 */
		inline R* GetDirect(UUID id)
		{
			auto iter = m_IDToResourceIndex.find(id);
			if (iter == m_IDToResourceIndex.end()) return nullptr;
			return &m_Resources[iter->second];
		}

		/** @brief Remove a resource from this manager.
		 * @param id ID of the resource to remove.
		 */
		inline virtual void Remove(UUID id) override
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

		/** @overload */
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

		/** @brief Type hash of the resource type managed by this manager */
		inline static uint32_t TypeHash = ResourceTypes::GetHash<R>();

	private:
		/** @brief Reserve capacity for managed resources.
		 * @param newCapacity Capacity to reserve.
		 *
		 * Creates a new array and moves each resource to the new array, then deletes the old array.
		 */
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