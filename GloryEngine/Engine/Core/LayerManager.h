#pragma once
#include "Layer.h"

#include <engine_visibility.h>

#include <vector>
#include <unordered_map>

#define LAYER_MANAGER GloryContext::GetLayerManager()

namespace Glory
{
	class IEngine;

	class LayerManager
	{
	public:
		GLORY_ENGINE_API LayerManager(IEngine* pEngine);
		GLORY_ENGINE_API virtual ~LayerManager();

		GLORY_ENGINE_API void AddLayer(const std::string& name);
		GLORY_ENGINE_API void Load();

		GLORY_ENGINE_API const Layer* GetLayerByName(const std::string& name) const;
		GLORY_ENGINE_API std::string LayerMaskToString(const LayerMask& layerMask) const;

		GLORY_ENGINE_API int GetLayerIndex(const Layer* pLayer) const;
		GLORY_ENGINE_API void GetAllLayerNames(std::vector<std::string_view>& names) const;
		GLORY_ENGINE_API const Layer* GetLayerAtIndex(int index) const;

		GLORY_ENGINE_API const size_t LayerCount() const;

	private:
		void CreateDefaultLayers();

	private:
		friend class GloryContext;
		friend class IEngine;
		IEngine* m_pEngine;
		std::vector<Layer> m_Layers;
		std::unordered_map<std::string, size_t> m_NameToLayer;
	};
}
