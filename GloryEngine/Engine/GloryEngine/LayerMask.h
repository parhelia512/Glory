#pragma once
#include <engine_visibility.h>

#include <cstdint>
#include <string>
#include <Reflection.h>

namespace Glory
{
	class IEngine;

	struct LayerMask
	{
		GLORY_ENGINE_API LayerMask() : m_Mask(0) {}
		GLORY_ENGINE_API LayerMask(uint64_t mask) : m_Mask(mask) {}

		GLORY_ENGINE_API operator uint64_t() const { return m_Mask; }
		GLORY_ENGINE_API LayerMask& operator |=(const LayerMask& other);
		GLORY_ENGINE_API LayerMask& operator ^=(const LayerMask& other);
		GLORY_ENGINE_API LayerMask operator &(const LayerMask& other) const;
		GLORY_ENGINE_API LayerMask& operator =(uint64_t mask);

		GLORY_ENGINE_API static LayerMask FromString(IEngine* pEngine, const std::string& names);

		REFLECTABLE(LayerMask, (uint64_t)(m_Mask))
	};
}
