#pragma once
#include <UUID.h>

#include <engine_visibility.h>

namespace Glory
{
	class IEngine;
	class MaterialData;

	class MaterialManager
	{
	public:
		GLORY_ENGINE_API MaterialManager(IEngine* pEngine);
		virtual ~MaterialManager() = default;

		virtual MaterialData* GetMaterial(UUID materialID) const = 0;
		virtual MaterialData* CreateRuntimeMaterial(UUID baseMaterial) = 0;
		virtual void DestroyRuntimeMaterials() = 0;

	protected:
		IEngine* m_pEngine;
	};
}
