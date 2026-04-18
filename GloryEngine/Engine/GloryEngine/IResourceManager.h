#pragma once
#include <cstdint>

namespace Glory
{
	class Resource;

	/** @brief Resource manager interface */
	class IResourceManager
	{
	public:
		/** @brief Type hash of the resource */
		virtual uint32_t Type() const = 0;
		/** @brief Add a resource to the manager */
		virtual Resource* Add(Resource* pResource) = 0;
		/** @brief Find a resource in the manager */
		virtual Resource* Get(UUID id) = 0;
		/** @brief Remove a resource from the manager */
		virtual void Remove(UUID id) = 0;
	};
}