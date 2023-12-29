#pragma once
#include "Object.h"
#include "GameSettings.h"
#include "Window.h"
#include "Glory.h"

#include <filesystem>

namespace Glory
{
	class Game
	{
	public:
		static Game& CreateGame(const GameSettings &gameSettings);
		static Game& GetGame();

		void Initialize();
		void RunGame();
		void Destroy();
		Engine* GetEngine();

		static void Quit();

		void SetAssetPath(const std::filesystem::path& path);
		void SetSettingsPath(const std::filesystem::path& path);
		static const std::string_view GetAssetPath();
		static const std::string_view GetSettingsPath();

		const ApplicationType& GetApplicationType() const;

	private:
		Game(const GameSettings& pGameState);
		Game();
		virtual ~Game();

	private:
		GameState* m_pGameState;
		Engine* m_pEngine;
		bool m_bIsRunning;
		bool m_bInitialized;

		static Game m_Game;
		static bool m_bGameCreated;
		std::string m_AssetPath;
		std::string m_SettingsPath;
		ApplicationType m_ApplicationType;
	};
}