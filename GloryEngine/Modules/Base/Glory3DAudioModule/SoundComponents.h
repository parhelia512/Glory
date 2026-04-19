#pragma once
#include <AssetReference.h>
#include <Reflection.h>

#include <SoundMaterialData.h>

namespace Glory
{
	struct SoundOccluder
	{
		SoundOccluder() {}

		REFLECTABLE(SoundOccluder,
			(ResourceReference<SoundMaterialData>)(m_MaterialAsset),
			(SoundMaterial)(m_Material)
		);
	};
}