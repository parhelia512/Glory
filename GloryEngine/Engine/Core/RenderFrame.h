#pragma once
#include "RenderData.h"
#include "CameraRef.h"
#include "LightData.h"

#include "Debug.h"

#include <engine_visibility.h>

#include <queue>

namespace Glory
{
	template<typename T>
	struct FrameData
	{
	public:
		FrameData(size_t max) : m_Counter(0), m_ActiveObjects(max) {}
		~FrameData()
		{
			m_ActiveObjects.clear();
			m_Counter = 0;
		}

		inline void push_back(const T& value)
		{
			if (m_Counter >= m_ActiveObjects.size())
			{
				//const std::string typeName = typeid(T).name();
				//m_pEngine->GetDebug().LogWarning(typeName + " overflow in render data!");
				return;
			}
			m_ActiveObjects[m_Counter] = std::move(value);
			++m_Counter;
		}

		inline size_t count() const
		{
			return m_Counter;
		}

		inline size_t size() const
		{
			return m_ActiveObjects.size();
		}

		inline const T& operator[](size_t index) const
		{
			return m_ActiveObjects[index];
		}

		inline T& operator[](size_t index)
		{
			return m_ActiveObjects[index];
		}

		inline const void* data() const
		{
			return m_ActiveObjects.data();
		}

		inline void reset()
		{
			m_Counter = 0;
		}

	private:
		std::vector<T> m_ActiveObjects;
		size_t m_Counter;
	};

	struct RenderFrame
	{
	public:
		GLORY_ENGINE_API RenderFrame(size_t maxLigts);
		GLORY_ENGINE_API ~RenderFrame();

		GLORY_ENGINE_API void Reset();

	public:
		std::vector<std::pair<glm::ivec2, UUID>> Picking;
		FrameData<LightData> ActiveLights;
		FrameData<glm::mat4> LightViews;
		FrameData<glm::mat4> LightProjections;
		std::vector<UUID> ActiveLightIDs;
	};

	enum RenderPassType : size_t
	{
		RP_Prepass,
		RP_CameraPrepass,
		RP_ObjectPass,
		RP_LateobjectPass,
		RP_CameraPostpass,
		RP_PreCompositePass,
		RP_CameraCompositePass,
		RP_PostCompositePass,
		RP_Postpass,
		RP_Postblit,
		RP_Count
	};
}
