#pragma once
#include "ProfilerThreadSample.h"

#include <engine_visibility.h>

#include <string>
#include <thread>
#include <functional>

namespace Glory
{
	class EngineProfiler
	{
	public:
		GLORY_ENGINE_API EngineProfiler();
		GLORY_ENGINE_API virtual ~EngineProfiler();

		GLORY_ENGINE_API void RegisterRecordCallback(std::function<void(const ProfilerThreadSample&)> callback);

		GLORY_ENGINE_API void BeginThread(const std::string& name);
		GLORY_ENGINE_API void EndThread();
		GLORY_ENGINE_API void BeginSample(const std::string& name);
		GLORY_ENGINE_API void EndSample();

		GLORY_ENGINE_API void EnableSampleCollecting(bool enabled);

	private:
		std::unordered_map<std::string, ProfilerThreadSample> m_CurrentThreadSamples;
		std::unordered_map<std::thread::id, std::string> m_ThreadIDToProfile;
		std::function<void(const ProfilerThreadSample&)> m_RecordCallback;
		bool m_SampleCollectingEnabled;
	};

	struct GLORY_ENGINE_API ProfileSample
	{
		ProfileSample(EngineProfiler* pProfiler, const std::string& name);
		~ProfileSample();

		EngineProfiler* Profiler;
	};
}
