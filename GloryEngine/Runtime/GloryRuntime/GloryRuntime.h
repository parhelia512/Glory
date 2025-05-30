#pragma once
#include "Visibility.h"

#include <UUID.h>
#include <memory>
#include <vector>
#include <string>
#include <chrono>

namespace std::filesystem
{
	class path;
}

namespace Glory
{
	class Engine;
	class RuntimeAssetManager;
	class RuntimeSceneManager;
	class RuntimePipelineManager;
	class RuntimeMaterialManager;
	class RendererModule;
	class GraphicsModule;
	class WindowModule;

	/** @brief Glory runtime */
	class GloryRuntime
	{
	public:
		/** @brief Constructor */
		GLORY_RUNTIME_API GloryRuntime(Engine* pEngine);
		/** @brief Destructor */
		GLORY_RUNTIME_API ~GloryRuntime();
		/** @brief Initialize the runtime */
		GLORY_RUNTIME_API void Initialize();
		/** @brief Run the runtime */
		GLORY_RUNTIME_API void Run();
		/** @brief Load an asset database at a path and append it to the current database */
		GLORY_RUNTIME_API void LoadAssetDatabase(const std::filesystem::path& path);
		/** @brief Load a scene and its assets and shaders */
		GLORY_RUNTIME_API void LoadScene(const UUID uuid);
		/** @brief Load an asset group at a path */
		GLORY_RUNTIME_API void LoadAssetGroup(const std::filesystem::path& path);
		/** @brief Load a shader pack at a path */
		GLORY_RUNTIME_API void LoadShaderPack(const std::filesystem::path& path);
		/** @brief Get the engine attached to this runtime */
		GLORY_RUNTIME_API Engine* GetEngine();
		/** @brief Set the data path */
		GLORY_RUNTIME_API void SetDataPath(const std::string& dataPath);
		/** @brief Get the data path */
		GLORY_RUNTIME_API std::string_view GetDataPath();
		/** @brief Set framerate limit */
		GLORY_RUNTIME_API void SetFramerateLimit(float limit);
		/** @brief Is the runtime running? */
		GLORY_RUNTIME_API bool IsRunning() const;

	private:
		/** @brief End of a frame */
		void EndFrame();

	private:
		Engine* m_pEngine;
		RendererModule* m_pRenderer;
		GraphicsModule* m_pGraphics;
		WindowModule* m_pWindows;
		std::unique_ptr<RuntimeAssetManager> m_AssetManager;
		std::unique_ptr<RuntimeSceneManager> m_SceneManager;
		std::unique_ptr<RuntimePipelineManager> m_PipelineManager;
		std::unique_ptr<RuntimeMaterialManager> m_MaterialManager;
		std::vector<std::filesystem::path> m_AppendedAssetDatabases;
		std::string m_DataPath;

		std::chrono::time_point<std::chrono::system_clock> m_LastRenderedFrame;
		float m_MaxFramerate = 300.0f;
		bool m_IsRunning = false;
	};
}
