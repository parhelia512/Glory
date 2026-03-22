#pragma once
#include "AssetLocation.h"
#include "ResourceMeta.h"

#include <engine_visibility.h>

#include <UUID.h>
#include <map>

namespace Glory
{
	class AssetDatabase
	{
	public:
		GLORY_ENGINE_API AssetDatabase();
		GLORY_ENGINE_API virtual ~AssetDatabase();

		GLORY_ENGINE_API void Initialize();
		GLORY_ENGINE_API void Destroy();

		GLORY_ENGINE_API bool GetAssetLocation(UUID uuid, AssetLocation& location);
		GLORY_ENGINE_API bool GetResourceMeta(UUID uuid, ResourceMeta& meta);
		GLORY_ENGINE_API UUID GetAssetUUID(const std::string& path);
		GLORY_ENGINE_API bool AssetExists(UUID uuid);
		GLORY_ENGINE_API bool AssetExists(const std::string& path);

		GLORY_ENGINE_API void GetAllAssetsOfType(uint32_t typeHash, std::vector<UUID>& out);
		GLORY_ENGINE_API void GetAllAssetsOfType(uint32_t typeHash, std::vector<std::string>& out);

		GLORY_ENGINE_API std::string GetAssetName(UUID uuid);

		GLORY_ENGINE_API void Clear();

		GLORY_ENGINE_API void SetIDAndName(Resource* pResource, UUID newID, const std::string& name);

		GLORY_ENGINE_API void SetAsset(AssetLocation& assetLocation, const ResourceMeta& meta);

		GLORY_ENGINE_API void Remove(UUID uuid);

		/** @brief Set the root path where assets are located */
		GLORY_ENGINE_API void SetAssetPath(const std::filesystem::path& path);
		/** @brief Set the root path where engine and module settings are located */
		GLORY_ENGINE_API void SetSettingsPath(const std::filesystem::path& path);

		/** @brief Get the root path where assets are located */
		GLORY_ENGINE_API const std::string_view GetAssetPath() const { return m_AssetPath; }
		/** @brief Get the root path where engine and module settings are located */
		GLORY_ENGINE_API const std::string_view GetSettingsPath() const { return m_SettingsPath; };

		/**
		 * @brief Get the ID of a scene by name
		 * @param name Name of the scene to find
		 * @returns ID of the scene or 0 if not found
		 */
		GLORY_ENGINE_API UUID FindSceneID(const std::string name) const;

		struct GLORY_ENGINE_API WriteLock
		{
		public:
			WriteLock(AssetDatabase* pDatabase);
			~WriteLock();

		private:
			static size_t m_LockCounter;
			AssetDatabase* m_pDatabase;
		};

		GLORY_ENGINE_API void SetEntryScene(UUID uuid);
		GLORY_ENGINE_API UUID GetEntryScene() const;

	private:
		bool m_IsWriting;
		bool m_IsReading;
		std::map<UUID, AssetLocation> m_AssetLocations;
		std::map<std::string, UUID> m_SceneNameToID;
		std::map<std::string, UUID> m_PathToUUID;
		std::map<UUID, ResourceMeta> m_Metas;
		std::map<uint32_t, std::vector<UUID>> m_AssetsByType;
		std::string m_AssetPath;
		std::string m_SettingsPath;
		UUID m_EntrySceneID;

		struct ReadLock
		{
		public:
			ReadLock(AssetDatabase* pDatabase);
			~ReadLock();

		private:
			static size_t m_LockCounter;
			AssetDatabase* m_pDatabase;
		};
	};
}
