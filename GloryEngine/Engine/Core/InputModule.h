#pragma once
#include "PlayerInput.h"

#include <engine_visibility.h>

#include <Module.h>

#include <glm/glm.hpp>

namespace Glory::Utils
{
	class BinaryStream;
}

namespace Glory
{
	class InputModule : public Module
	{
	public:
		GLORY_ENGINE_API InputModule();
		GLORY_ENGINE_API virtual ~InputModule();

		GLORY_ENGINE_API virtual const std::type_info& GetModuleType() override;

		GLORY_ENGINE_API bool OnInput(InputEvent& event);
		GLORY_ENGINE_API void OnCursor(CursorEvent& event);
		GLORY_ENGINE_API bool OnText(TextEvent& event);

		GLORY_ENGINE_API size_t AddPlayer();
		GLORY_ENGINE_API void RemovePlayer(size_t playerIndex);

		GLORY_ENGINE_API void ReadInputData(YAML::Node& node);

		GLORY_ENGINE_API void ClearInputData();

		GLORY_ENGINE_API void SetPlayerInputMode(const size_t playerIndex, const std::string& inputMode);

		GLORY_ENGINE_API const UUID GetDeviceUUID(const InputDeviceType deviceType, const size_t deviceID) const;
		GLORY_ENGINE_API InputDevice* GetInputDevice(const UUID deviceID);

		GLORY_ENGINE_API InputMode* GetInputMode(const std::string& name);

		GLORY_ENGINE_API bool& InputBlocked();

		GLORY_ENGINE_API PlayerInput* GetPlayer(size_t playIndex);
		GLORY_ENGINE_API const PlayerInput* GetPlayer(size_t playIndex) const;
		GLORY_ENGINE_API float GetAxis(size_t playerIndex, const std::string& inputMap, const std::string& actionName) const;
		GLORY_ENGINE_API float GetAxisDelta(size_t playerIndex, const std::string& inputMap, const std::string& actionName) const;
		GLORY_ENGINE_API bool GetBool(size_t playerIndex, const std::string& inputMap, const std::string& actionName) const;
		GLORY_ENGINE_API glm::vec2 GetCursorPos(size_t playerIndex) const;
		GLORY_ENGINE_API glm::vec2 GetCursorScrollDelta(size_t playerIndex) const;
		GLORY_ENGINE_API bool IsCursorDown(size_t playerIndex) const;

		GLORY_ENGINE_API void FreeDevice(const UUID deviceId);

		GLORY_ENGINE_API const UUID FindAvailableInputDevice(const InputDeviceType deviceType) const;

		GLORY_ENGINE_API void SetCursorBounds(const glm::vec4& bounds);
		GLORY_ENGINE_API const glm::vec4& GetCursorBounds();

		GLORY_ENGINE_API void SetScreenScale(const glm::vec2& scale);
		GLORY_ENGINE_API const glm::vec2& GetScreenScale();

	protected:
		virtual void OnInitialize() {};
		virtual void OnCleanup() {};
		virtual void OnUpdate() {};

		GLORY_ENGINE_API virtual void OnProcessData() override;

	private:
		GLORY_ENGINE_API virtual void Initialize() override;
		GLORY_ENGINE_API virtual void PostInitialize() override;
		GLORY_ENGINE_API virtual void Cleanup() override;
		GLORY_ENGINE_API virtual void Update() override;
		GLORY_ENGINE_API virtual void OnBeginFrame() override;

		void ReadInputModes(YAML::Node& node);
		void ReadInputMaps(YAML::Node& node);

	private:
		std::map<UUID, InputDevice> m_InputDevices;
		UUID m_DefaultMouseDeviceIndex;
		UUID m_DefaultKeyboardDeviceIndex;

		std::vector<PlayerInput> m_Players;
		std::map<std::string, InputMode> m_InputModes;
		/* First map is by InputMap name, second by InputMode name */
		std::map<std::string, std::map<std::string, InputMap>> m_InputMaps;

		bool m_InputBlocked;
		glm::vec4 m_CursorBounds;
		glm::vec2 m_ScreenScale;
	};
}
