#pragma once
#include <engine_visibility.h>

#include <string>
#include <chrono>
#include <vector>

namespace Glory
{
	class ProfilerSample
	{
	public:
		GLORY_ENGINE_API ProfilerSample(const std::string& name);
		GLORY_ENGINE_API virtual ~ProfilerSample();

		GLORY_ENGINE_API const std::string& Name();
		GLORY_ENGINE_API size_t SubSampleCount();
		GLORY_ENGINE_API ProfilerSample* GetSubSample(size_t index);

		template<typename T, typename Ratio>
		inline const T GetDuration()
		{
			std::chrono::duration<T, Ratio> duration = m_SampleEnd - m_SampleStart;
			return duration.count();
		}

	private:
		void BeginSubSample(const std::string& name);
		void EndSample();

	private:
		friend class ProfilerThreadSample;
		std::string m_Name;

		std::chrono::time_point<std::chrono::steady_clock> m_SampleStart;
		std::chrono::time_point<std::chrono::steady_clock> m_SampleEnd;

		bool m_Complete;

		std::vector<ProfilerSample> m_SubSamples;
	};
}
