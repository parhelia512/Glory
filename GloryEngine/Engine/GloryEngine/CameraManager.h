#pragma once
#include "CameraRef.h"

#include <engine_visibility.h>

#include <unordered_map>

namespace Glory
{
	class Camera;
	class IEngine;

	class CameraManager
	{
	public:
		GLORY_ENGINE_API CameraManager(IEngine* pEngine);
		GLORY_ENGINE_API virtual ~CameraManager();

		GLORY_ENGINE_API CameraRef GetNewOrUnusedCamera();
		GLORY_ENGINE_API void SetUnused(Camera* pCamera);
		GLORY_ENGINE_API Camera* GetCamera(UUID uuid);

	private:
		friend class IEngine;
		void Cleanup();

	private:
		IEngine* m_pEngine;
		std::vector<Camera> m_Cameras;
		std::vector<size_t> m_UnusedCameraIndices;
		std::unordered_map<UUID, size_t> m_IDToCamera;
	};
}
