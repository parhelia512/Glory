#pragma once
#include <cstdint>

#include <engine_visibility.h>

namespace Glory
{
	class IEngine;

	class GameTime
	{
	public:
		GLORY_ENGINE_API GameTime(IEngine* pEngine);
		GLORY_ENGINE_API virtual ~GameTime();

		GLORY_ENGINE_API void Initialize();

		GLORY_ENGINE_API void BeginFrame();
		GLORY_ENGINE_API void EndFrame();

		GLORY_ENGINE_API const float GetTime() const;
		GLORY_ENGINE_API const float GetUnscaledTime() const;
		GLORY_ENGINE_API const float GetDeltaTime() const;
		GLORY_ENGINE_API const float GetUnscaledDeltaTime() const;
		GLORY_ENGINE_API const float GetTimeScale() const;
		GLORY_ENGINE_API const float GetFrameRate() const;
		GLORY_ENGINE_API const int GetTotalFrames() const;
		GLORY_ENGINE_API void SetTimeScale(float scale);

		GLORY_ENGINE_API static uint64_t Now();
		GLORY_ENGINE_API static float TimeSinceSeconds(uint64_t timestamp);

	private:
		friend class IEngine;

		IEngine* m_pEngine;
		float m_TimeScale = 1.0f;
		int m_TotalFrames = 0;

		double m_DeltaTime = 0.0f;
		double m_LastTime = 0.0f;
		uint64_t m_StartTime = 0;
	};
}
