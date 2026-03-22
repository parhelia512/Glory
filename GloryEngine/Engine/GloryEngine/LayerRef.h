#pragma once
#include <string>

#include <engine_visibility.h>

namespace Glory::Utils::Reflect
{
	struct TypeData;
}

namespace Glory
{
	class LayerManager;
	struct Layer;

	struct LayerRef
	{
	public:
		GLORY_ENGINE_API LayerRef();
		GLORY_ENGINE_API LayerRef(uint32_t layerIndex);

		uint32_t m_LayerIndex;

	public:
		GLORY_ENGINE_API bool operator==(const LayerRef& other) const;
		GLORY_ENGINE_API bool operator==(LayerRef& other);

		GLORY_ENGINE_API const Layer* Layer(LayerManager* pManager) const;

		GLORY_ENGINE_API static const Glory::Utils::Reflect::TypeData* GetTypeData();

	};
}
