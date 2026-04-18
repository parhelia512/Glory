#pragma once
#include <cstdint>

namespace Glory
{
	class Resource;

	class IResourceManager
	{
	public:
		virtual uint32_t Type() const = 0;
		virtual Resource* Add(Resource* pResource) = 0;
		virtual Resource* Get(UUID id) = 0;
		virtual void Remove(UUID id) = 0;
	};
}