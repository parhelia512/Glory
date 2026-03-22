#pragma once
#include "KeyEnums.h"

#include <engine_visibility.h>

#include <glm/vec2.hpp>

REFLECTABLE_ENUM_NS(Glory, InputDeviceType, Keyboard, Mouse, Gamepad)
REFLECTABLE_ENUM_NS(Glory, InputMappingType, Bool, Float)
REFLECTABLE_ENUM_NS(Glory, InputState, KeyPressed, KeyDown, KeyUp, Axis)
REFLECTABLE_ENUM_NS(Glory, AxisBlending, Jump, Lerp, SLerp)

namespace Glory
{
	struct GLORY_ENGINE_API InputDevice
	{
		InputDevice(const char* name, const InputDeviceType deviceType, size_t deviceID);

		const char* m_Name;
		const InputDeviceType m_DeviceType;
		const size_t m_DeviceID;
		int m_PlayerIndex;
	};

	struct InputMode
	{
		GLORY_ENGINE_API InputMode(const std::string name);
		GLORY_ENGINE_API InputMode(InputMode&& other) noexcept;
		const std::string m_Name;
		std::vector<InputDeviceType> m_DeviceTypes;

		static InputMode None;
	};

	struct InputEvent
	{
		InputDeviceType InputDeviceType;
		size_t SourceDeviceID;
		size_t KeyID;
		InputState State;
		float Value;
		float Delta;
		KeyboardMod KeyMods;
	};

	struct CursorEvent
	{
		enum Type
		{
			Motion,
			Button,
			Scroll
		};

		Type Type;
		InputDeviceType InputDeviceType;
		size_t SourceDeviceID;
		glm::vec2 Cursor;
		bool IsDelta;
		bool IsDown;
	};

	struct TextEvent
	{
		InputDeviceType InputDeviceType;
		char Character;
	};

	struct KeyBindingCompact
	{
		InputDeviceType m_DeviceType;
		size_t m_KeyID;
		bool m_IsAxis;
	};

	struct KeyBinding
	{
		GLORY_ENGINE_API KeyBinding(const std::string bindingPath, const KeyBindingCompact& compact);
		GLORY_ENGINE_API KeyBinding(const std::string bindingPath);
		GLORY_ENGINE_API KeyBinding(const KeyBinding& other) noexcept;

		GLORY_ENGINE_API KeyBinding& operator=(const KeyBinding&& other) noexcept;

		GLORY_ENGINE_API bool CheckEvent(InputEvent& e);

		GLORY_ENGINE_API KeyBindingCompact Compact();

		std::string m_BindingPath;
		InputDeviceType m_DeviceType;
		size_t m_KeyID;
		bool m_IsAxis;
	};

	struct InputBinding
	{
		GLORY_ENGINE_API InputBinding(const std::string name, const InputState inputState, const float multiplier, const bool mapDeltaToValue, const KeyBinding keybinding);

		const std::string m_Name;
		const InputState m_State;
		const float m_Multiplier;
		const bool m_MapDeltaToValue;
		KeyBinding m_KeyBinding;

	private:
		//InputBinding(const InputBinding& other) = delete;
		//InputBinding& operator=(const InputBinding&& other) = delete;
	};

	struct InputAction
	{
		GLORY_ENGINE_API InputAction(const std::string, const InputMappingType mappingType, const AxisBlending axisBlending, const float blendSpeed);

		const std::string m_Name;
		const InputMappingType m_MappingType;
		const AxisBlending m_Blending;
		const float m_BlendSpeed;
		std::vector<InputBinding> m_Bindings;

	private:
		//InputAction(const InputAction& other) = delete;
		//InputAction& operator=(const InputAction&& other) = delete;
	};

	struct InputMap
	{
		GLORY_ENGINE_API InputMap(const std::string name);

		const std::string m_Name;
		std::map<std::string, InputAction> m_Actions;

	private:
		//InputMap(const InputMap& other) = delete;
		//InputMap& operator=(const InputMap&& other) = delete;
	};
}
