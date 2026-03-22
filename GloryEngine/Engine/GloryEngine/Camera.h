#pragma once
#include "CameraRef.h"
#include "LayerMask.h"

#include <engine_visibility.h>

#include <glm/glm.hpp>
#include <unordered_map>

namespace Glory
{
	class Camera
	{
	public:
		GLORY_ENGINE_API bool SetBaseResolution(uint32_t width, uint32_t height);
		GLORY_ENGINE_API const glm::uvec2& GetBaseResolution() const;
		GLORY_ENGINE_API bool SetResolutionScale(float width, float height);
		GLORY_ENGINE_API void SetPerspectiveProjection(float halfFOV, float near, float far, bool force=false);
		GLORY_ENGINE_API void SetOutput(bool output, int x, int y);
		GLORY_ENGINE_API bool IsOutput() const;
		GLORY_ENGINE_API void SetOrthographicProjection(float near, float far, bool force=false);
		GLORY_ENGINE_API void SetView(const glm::mat4& view);
		GLORY_ENGINE_API void SetPriority(int priority);
		GLORY_ENGINE_API void SetLayerMask(const LayerMask& layerMask);
		GLORY_ENGINE_API void SetClearColor(const glm::vec4& clearColor);
		GLORY_ENGINE_API void SetUserData(const std::string& name, void* data);
		GLORY_ENGINE_API bool IsResolutionDirty();
		GLORY_ENGINE_API bool IsPerspectiveDirty();
		GLORY_ENGINE_API void SetResolutionDirty(bool dirty=true);
		GLORY_ENGINE_API void SetPerspectiveDirty(bool dirty=true);
		GLORY_ENGINE_API glm::vec3 GetFocusPosition(const BoundingSphere& boundingSphere) const;
		GLORY_ENGINE_API void Focus(const BoundingSphere& boundingSphere);

		GLORY_ENGINE_API const glm::uvec2& GetResolution() const;
		GLORY_ENGINE_API const glm::mat4& GetView() const;
		GLORY_ENGINE_API const glm::mat4& GetViewOffset() const;
		GLORY_ENGINE_API const glm::mat4& GetProjection() const;
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

	private:
		Camera(uint32_t width, uint32_t height);

	private:
		friend class CameraManager;
		bool m_IsInUse;
		bool m_IsOrtho;
		bool m_Output;
		bool m_ResolutionDirty;
		bool m_PerspectiveDirty;
		int m_Priority;
		float m_Near;
		float m_Far;
		float m_HalfFOV;
		LayerMask m_LayerMask;

		glm::mat4 m_View;
		glm::mat4 m_ViewOffset;
		glm::mat4 m_Projection;
		glm::uvec2 m_BaseResolution;
		glm::vec2 m_ResolutionScale;
		glm::uvec2 m_Resolution;
		glm::ivec2 m_OutputOffset;

		glm::vec4 m_ClearColor;

		std::unordered_map<std::string, void*> m_UserDatas;
		std::unordered_map<std::string, uint64_t> m_UserHandles;

		UUID m_UUID;
	};
}
