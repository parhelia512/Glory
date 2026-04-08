#pragma once
#include <engine_visibility.h>

#include <string>
#include <vector>
#include <mutex>
#include <functional>

#ifndef GLORY_NO_DEBUG_LINES

#include <glm/glm.hpp>

#endif

namespace Glory
{
	class Renderer;
	class Console;
	class GameTime;
	class WindowModule;
	class ICaptureHandler;

	class Debug
	{
	public:
		GLORY_ENGINE_API Debug(Console* pConsole) : m_pConsole(pConsole), m_pWindowModule(nullptr), m_pCaptureHandler(nullptr) {}
		GLORY_ENGINE_API ~Debug() = default;

	public:
		enum class LogLevel
		{
			/// <summary>
			/// Normal debug info for debugging.
			/// </summary>
			Info,
			/// <summary>
			/// A notice for things that still attention, bug fixes, changes, additions etc.
			/// </summary>
			Notice,
			/// <summary>
			/// A warning will be displayed in yellow.
			/// </summary>
			Warning,
			/// <summary>
			/// An error will be displayed in red.
			/// </summary>
			Error,
			/// <summary>
			/// A fatal error will display the error in a seperate window and then close the game.
			/// </summary>
			FatalError,
		};

	public:
		GLORY_ENGINE_API void Log(const std::string& message, const LogLevel& logLevel, bool bIncludeTimeStamp = true);
		GLORY_ENGINE_API void LogInfo(const std::string& message, bool bIncludeTimeStamp = true);
		GLORY_ENGINE_API void LogNotice(const std::string& message, bool bIncludeTimeStamp = true);
		GLORY_ENGINE_API void LogWarning(const std::string& message, bool bIncludeTimeStamp = true);
		GLORY_ENGINE_API void LogError(const std::string& message, bool bIncludeTimeStamp = true);
		GLORY_ENGINE_API void LogFatalError(const std::string& message, bool bIncludeTimeStamp = true);

		GLORY_ENGINE_API void LogOnce(const std::string& key, const std::string& message, const LogLevel& logLevel, bool bIncludeTimeStamp = true);

		GLORY_ENGINE_API void SetWindowModule(WindowModule* pWindows);

		GLORY_ENGINE_API void SetCaptureHandler(ICaptureHandler* pHandler);

		GLORY_ENGINE_API void StartCapture();
		GLORY_ENGINE_API void EndCapture();

#ifndef GLORY_NO_DEBUG_LINES

		GLORY_ENGINE_API void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float time = 0.1f);
		GLORY_ENGINE_API void DrawLineQuad(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4, const glm::vec4& color, float time = 0.1f);
		GLORY_ENGINE_API void DrawWireCube(const glm::vec3& position, const glm::vec3& extends, const glm::vec4& color, float time = 0.1f);
		GLORY_ENGINE_API void DrawRay(const glm::vec3& start, const glm::vec3& dir, const glm::vec4& color, float length = 1.0f, float time = 0.1f);
		GLORY_ENGINE_API void SubmitLines(Renderer* pRenderer, GameTime* pTime);

#endif

	private:
		friend class IEngine;

		std::mutex m_Lock;
		Console* m_pConsole;
		WindowModule* m_pWindowModule;

		ICaptureHandler* m_pCaptureHandler;

#ifndef GLORY_NO_DEBUG_LINES

		struct DebugLine
		{
			glm::vec3 Start;
			glm::vec3 End;
			glm::vec4 Color;
			float Time;
		};
		std::vector<DebugLine> m_DebugLines;

#endif
	};
}