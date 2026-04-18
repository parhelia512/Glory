#pragma once
#include "Resource.h"
#include "MeshData.h"

#include <engine_visibility.h>

#include <vector>

namespace Glory
{
	class ModelData : public Resource
	{
	public:
		GLORY_ENGINE_API ModelData();
		GLORY_ENGINE_API ModelData(ModelData&&) noexcept = default;
		GLORY_ENGINE_API ModelData& operator=(ModelData&&) noexcept = default;
		GLORY_ENGINE_API virtual ~ModelData();

	private:
		virtual void References(IEngine* pEngine, std::vector<UUID>& references) const override {}
	};
}