#pragma once
#include "ProfilerThreadSample.h"

#include <string>
#include <thread>
#include <functional>

namespace Glory
{
	class EngineProfiler
	{
	public:
		EngineProfiler();
		virtual ~EngineProfiler();

		void RegisterRecordCallback(std::function<void(const ProfilerThreadSample&)> callback);

		void BeginThread(const std::string& name);
		void EndThread();
		void BeginSample(const std::string& name);
		void EndSample();

		void EnableSampleCollecting(bool enabled);

	private:
		std::unordered_map<std::string, ProfilerThreadSample> m_CurrentThreadSamples;
		std::unordered_map<std::thread::id, std::string> m_ThreadIDToProfile;
		std::function<void(const ProfilerThreadSample&)> m_RecordCallback;
		bool m_SampleCollectingEnabled;
	};

	struct ProfileSample
	{
		ProfileSample(EngineProfiler* pProfiler, const std::string& name);
		~ProfileSample();

		EngineProfiler* Profiler;
	};
}
