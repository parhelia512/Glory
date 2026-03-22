#pragma once
#include "Input.h"

#include <engine_visibility.h>

#include <string_view>
#include <map>
#include <vector>
#include <filesystem>
#include <glm/vec2.hpp>

#include <UUID.h>

namespace Glory
{
	class InputModule;
	struct InputMap;

	struct PlayerInputData
	{
		GLORY_ENGINE_API PlayerInputData(InputMap* pInputMap);

		InputMap* m_InputMap;
		/* The current values of all axes */
		std::map<std::string, float> m_AxisValueLeft;
		std::map<std::string, float> m_AxisValueRight;
		std::map<std::string, float> m_AxisDesiredValueLeft;
		std::map<std::string, float> m_AxisDesiredValueRight;
		/* The axis deltas from the current frame */
		std::map<std::string, float> m_AxisDeltas;
		/* Actions that were triggered this frame */
		std::vector<std::string_view> m_TriggeredActions;
		/* Used for KeyDown actions */
		std::map<std::string_view, std::string_view> m_DownStates;
		/* Used for KeyPressed actions */
		std::map<std::string_view, bool> m_PressedStates;
		/* When MapDeltaToValue is set the value should also be cleared */
		std::vector<std::string> m_ToClearValues;
	};

	class PlayerInput
	{
	public:
		GLORY_ENGINE_API PlayerInput(InputModule* pInputModule, size_t playerIndex);
		GLORY_ENGINE_API virtual ~PlayerInput();

		GLORY_ENGINE_API std::string_view InputMode();

		GLORY_ENGINE_API void HandleInputEvent(InputEvent& event);
		GLORY_ENGINE_API void HandleCursorEvent(CursorEvent& event);

		/* Clears all data from actions during the current frame */
		GLORY_ENGINE_API void ClearActions();

		GLORY_ENGINE_API const float GetAxis(const std::string& inputMap, const std::string& actionName) const;
		GLORY_ENGINE_API const float GetAxisDelta(const std::string& inputMap, const std::string& actionName) const;
		GLORY_ENGINE_API const bool GetBool(const std::string& inputMap, const std::string& actionName) const;
		GLORY_ENGINE_API const glm::vec2& GetCursorPos() const;
		GLORY_ENGINE_API const glm::vec2& GetCursorScrollDelta() const;
		GLORY_ENGINE_API bool IsCursorDown() const;

		GLORY_ENGINE_API void Unbind();

		GLORY_ENGINE_API void Update();

		/*template<typename T>
		void WriteData(std::string_view name, T value)
		{
			if (sizeof(T) > 4)
				throw new std::exception("PlayerInput.WriteData: Data type cannot be more than 4 bytes!");

			if (m_InputData.find(name) == m_InputData.end())
				m_InputData.emplace(name, "\0");
			memcpy(&m_InputData.at(name), &value, sizeof(T));
		}

		template<typename T>
		void ReadData(std::string_view name, T& value)
		{
			if (sizeof(T) > 4)
				throw new std::exception("PlayerInput.ReadData: Data type cannot be more than 4 bytes!");

			if (m_InputData.find(name) == m_InputData.end())
				return;

			memcpy(&value, &m_InputData.at(name), sizeof(T));
		}*/

	private:
		void TriggerAction(PlayerInputData& inputData, InputAction& inputAction, InputBinding& inputBinding, InputEvent& e);
		void MapOnFloat(PlayerInputData& inputData, InputAction& inputAction, InputBinding& inputBinding, InputEvent& e);

		float Lerp(float a, float b, float t);
		float SLerp(float a, float b, float t);

	private:
		friend class InputModule;
		InputModule* m_pInputModule;
		size_t m_PlayerIndex;
		std::string_view m_InputMode;
		std::vector<PlayerInputData> m_InputData;
		std::vector<UUID> m_ClaimedDevices;
		std::vector<InputDeviceType> m_ClaimedDeviceTypes;
		glm::vec2 m_CursorPos;
		glm::vec2 m_CursorScrollDelta;
		bool m_CursorDown;
	};
}
