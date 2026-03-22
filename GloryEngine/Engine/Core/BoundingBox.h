#pragma once
#include "Entity.h"

#include <engine_visibility.h>

#include <glm/glm.hpp>

namespace Glory
{
	class IEngine;

	struct BoundingBox
	{
		BoundingBox() :
			m_Center(), m_HalfExtends() {
		}

		BoundingBox(const glm::vec4& center, const glm::vec4 halfExtends) :
			m_Center(center), m_HalfExtends(halfExtends) {
		}

		glm::vec4 m_Center;
		glm::vec4 m_HalfExtends;
	};

	struct AABB
	{
		GLORY_ENGINE_API AABB(const glm::vec3& origin={});
		GLORY_ENGINE_API AABB(const glm::vec3& min, const glm::vec3& max);

		glm::vec3 m_Min;
		glm::vec3 m_Max;

		GLORY_ENGINE_API void Combine(const BoundingBox& bounds, const glm::mat4& transform);
	};

	struct BoundingSphere
	{
		GLORY_ENGINE_API BoundingSphere(const glm::vec3& center={}, float radius=1.0f);

		union
		{
			glm::vec4 m_CenterAndRadius;
			struct
			{
				glm::vec3 m_Center;
				float m_Radius;
			};
		};

		GLORY_ENGINE_API void Combine(const BoundingSphere& bounds, const glm::mat4& transform);
	};

	GLORY_ENGINE_API AABB GenerateAABB(Entity entity, IEngine* pEngine, const glm::vec3& origin={});
	GLORY_ENGINE_API BoundingSphere GenerateBoundingSphere(Entity entity, IEngine* pEngine, const glm::vec3& defaultCenter={}, float defaultRadius=1.0f);
}
