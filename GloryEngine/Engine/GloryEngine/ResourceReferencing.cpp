#include "ResourceReferencing.h"
#include "Resources.h"

#include <GloryAssert.h>

namespace Glory
{
	Resources* ResourcesInstance = nullptr;

	void SetResourcesInstance(Resources* pResources)
	{
		ResourcesInstance = pResources;
	}

	void AddResourceReference(UUID id)
	{
		GLORY_ASSERT(ResourcesInstance != nullptr, "Missing resources instance");
		ResourcesInstance->AddReference(id);
	}

	void RemoveResourceReference(UUID id)
	{
		GLORY_ASSERT(ResourcesInstance != nullptr, "Missing resources instance");
		ResourcesInstance->RemoveReference(id);
	}
}
