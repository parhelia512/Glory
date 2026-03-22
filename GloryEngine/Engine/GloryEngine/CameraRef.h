#pragma once
#include "LayerMask.h"
#include "BoundingBox.h"

#include <engine_visibility.h>

#include <UUID.h>

#include <glm/glm.hpp>

namespace Glory
{
	class CameraManager;

	struct CameraRef
	{
	public:
		GLORY_ENGINE_API CameraRef();
		GLORY_ENGINE_API CameraRef(std::nullptr_t);

		GLORY_ENGINE_API bool operator==(const CameraRef& other) const;

	public:
		GLORY_ENGINE_API void SetBaseResolution(uint32_t width, uint32_t height);
		GLORY_ENGINE_API const glm::uvec2& GetBaseResolution() const;
		GLORY_ENGINE_API void SetResolutionScale(float width, float height);
		GLORY_ENGINE_API void SetPerspectiveProjection(float halfFOV, float near, float far);
		GLORY_ENGINE_API void SetOutput(bool output, int x, int y);
		GLORY_ENGINE_API bool IsOutput() const;
		GLORY_ENGINE_API void SetOrthographicProjection(float near, float far);
		GLORY_ENGINE_API void SetView(const glm::mat4& view);
		GLORY_ENGINE_API void SetPriority(int priority);
		GLORY_ENGINE_API void SetLayerMask(const LayerMask& layerMask);
		GLORY_ENGINE_API void SetClearColor(const glm::vec4& clearColor);
		GLORY_ENGINE_API void SetUserData(const std::string& name, void* data);
		GLORY_ENGINE_API bool IsResolutionDirty();
		GLORY_ENGINE_API bool IsPerspectiveDirty();
		GLORY_ENGINE_API void SetResolutionDirty(bool dirty=true);
		GLORY_ENGINE_API void SetPerspectiveDirty(bool dirty=true);
		GLORY_ENGINE_API void Focus(const BoundingSphere& boundingSphere);

		GLORY_ENGINE_API const glm::uvec2& GetResolution() const;
		GLORY_ENGINE_API const glm::mat4& GetView() const;
		GLORY_ENGINE_API glm::mat4 GetViewInverse() const;
		GLORY_ENGINE_API const glm::mat4& GetProjection() const;
		GLORY_ENGINE_API glm::mat4 GetProjectionInverse() const;
		GLORY_ENGINE_API glm::mat4 GetFinalView() const;

		GLORY_ENGINE_API float* GetViewPointer();
		GLORY_ENGINE_API float* GetProjectionPointer();

		GLORY_ENGINE_API int GetPriority() const;
		GLORY_ENGINE_API const glm::vec4& GetClearColor() const;
		GLORY_ENGINE_API const LayerMask& GetLayerMask() const;

		GLORY_ENGINE_API uint64_t& GetUserHandle(const std::string& name);

		GLORY_ENGINE_API float GetNear() const;
		GLORY_ENGINE_API float GetFar() const;
		GLORY_ENGINE_API UUID GetUUID() const;

		GLORY_ENGINE_API void Free();

	private:
		CameraRef(CameraManager* pManager, UUID uuid);

	private:
		friend class CameraManager;
		CameraManager* m_pManager;
		UUID m_CameraID;
	};
}
