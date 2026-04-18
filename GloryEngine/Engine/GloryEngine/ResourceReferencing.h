#pragma once
#include <engine_visibility.h>

#include <UUID.h>

namespace Glory
{
	class Resources;

	/** @brief Wrapper function for setting the current @ref Resources instance. */
	extern "C" GLORY_ENGINE_API void SetResourcesInstance(Resources* pResources);
	/** @brief Wrapper function for incrementing the reference counter of a resource by ID. */
	extern "C" GLORY_ENGINE_API void AddResourceReference(UUID id);
	/** @brief Wrapper function for decrementing the reference counter of a resource by ID. */
	extern "C" GLORY_ENGINE_API void RemoveResourceReference(UUID id);
}
