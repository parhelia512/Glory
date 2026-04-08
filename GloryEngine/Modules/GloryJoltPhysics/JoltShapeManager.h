#pragma once
#include "jolt_visibility.h"
#include "Shapes.h"

#include <UUID.h>
#include <map>

namespace JPH
{
	class Shape;
}

namespace Glory
{
	struct ShapeData
	{
		ShapeData(JPH::Shape* pShape, UUID uuid);

		const JPH::Shape* m_pShape;
		const UUID m_UUID;
	};

	class JoltShapeManager
	{
	public:
		JoltShapeManager();
		~JoltShapeManager();

		GLORY_JOLT_API UUID CreateShape(const Shape& shape);
		GLORY_JOLT_API void DestroyShape(const UUID shapeId);

		GLORY_JOLT_API const ShapeData* GetShape(const UUID shapeId) const;

		GLORY_JOLT_API void Clear();

	private:
		std::map<UUID, ShapeData> m_pShapes;
	};
}
