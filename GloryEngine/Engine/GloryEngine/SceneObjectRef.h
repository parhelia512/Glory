#pragma once
#include "Entity.h"

#include <engine_visibility.h>

#include <Reflection.h>

namespace Glory
{
	class SceneManager;

	class SceneObjectRef
	{
	public:
		GLORY_ENGINE_API SceneObjectRef();
		GLORY_ENGINE_API SceneObjectRef(UUID sceneUUID, UUID objectUUID);
		GLORY_ENGINE_API virtual ~SceneObjectRef();

		GLORY_ENGINE_API const UUID SceneUUID() const;
		GLORY_ENGINE_API UUID* SceneUUIDMember();
		GLORY_ENGINE_API const UUID ObjectUUID() const;
		GLORY_ENGINE_API UUID* ObjectUUIDMember();
		GLORY_ENGINE_API Entity GetEntity(SceneManager* pScenes);

		GLORY_ENGINE_API bool operator==(const SceneObjectRef& other) const;
		GLORY_ENGINE_API bool operator==(const SceneObjectRef&& other) const;

	protected:
		REFLECTABLE(SceneObjectRef, (UUID)(m_SceneUUID), (UUID)(m_ObjectUUID))
	};
}
