#pragma once
#include "ProfilerSample.h"
#include <string>
#include <vector>

namespace Glory
{
	class ProfilerThreadSample
	{
	public:
		GLORY_ENGINE_API ProfilerThreadSample();
		GLORY_ENGINE_API ProfilerThreadSample(const std::string& name);
		GLORY_ENGINE_API virtual ~ProfilerThreadSample();

		GLORY_ENGINE_API const std::string& Name() const;
		GLORY_ENGINE_API size_t SampleCount() const;
		GLORY_ENGINE_API ProfilerSample* GetSample(size_t index);

		template<typename T, typename Ratio>
		inline const T GetDuration() const
		{
			std::chrono::duration<T, Ratio> duration = m_End - m_Start;
			return duration.count();
		}

	private:
		void BeginSample(const std::string& name);
		void EndSample();
		void Clear();

	private:
		friend class EngineProfiler;
		std::string m_Name;
		std::vector<ProfilerSample> m_Samples;
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
		std::chrono::time_point<std::chrono::steady_clock> m_End;
	};
}