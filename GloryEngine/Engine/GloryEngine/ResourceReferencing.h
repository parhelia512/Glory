#pragma once
#include <engine_visibility.h>

#include <UUID.h>

namespace Glory
{
	class Resources;

	extern "C" GLORY_ENGINE_API void SetResourcesInstance(Resources* pResources);
	extern "C" GLORY_ENGINE_API void AddResourceReference(UUID id);
	extern "C" GLORY_ENGINE_API void RemoveResourceReference(UUID id);
}