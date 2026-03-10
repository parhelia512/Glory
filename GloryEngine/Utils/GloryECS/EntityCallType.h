#pragma once
#include <cstdint>

namespace Glory::Utils::ECS
{
	enum class EntityCallType : uint8_t
	{
		OnAdd,
		OnRemove,
		OnValidate,
		OnActivate,
		OnDeactivate,
		OnEnableDraw,
		OnDisableDraw,
		OnDirty,
		Start,
		Stop,
		PreUpdate,
		Update,
		PostUpdate,
		PreDraw,
		Draw,
		PostDraw,

		Count
	};
}