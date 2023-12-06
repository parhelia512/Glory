#pragma once
#include "CameraManager.h"
#include "UUIDRemapper.h"

namespace Glory::Utils::Reflect
{
	class Reflect;
}

namespace Glory
{
	class Debug;
	class Game;
	class AssetDatabase;
	class AssetManager;
	class ResourceTypes;
	class Serializers;
	class DisplayManager;
	class Console;
	class LayerManager;
	class ObjectManager;
	class ShaderManager;

	class GloryContext
	{
	public:
		static Game* GetGame();
		static CameraManager* GetCameraManager();
		static AssetDatabase* GetAssetDatabase();
		static ResourceTypes* GetResourceTypes();
		static Serializers* GetSerializers();
		static DisplayManager* GetDisplayManager();
		static Console* GetConsole();
		static LayerManager* GetLayerManager();
		static ShaderManager* GetShaderManager();
		static Debug* GetDebug();
		static void SetContext(GloryContext* pContext);
		static GloryContext* GetContext();
		static GloryContext* CreateContext();
		static void DestroyContext();

		template<class T>
		static void AddUserContext(T* pUserContext)
		{
			AddUserContext(ResourceType::GetHash(typeid(T)), (void*)pUserContext);
		}

		template<class T>
		static T* GetUserContext()
		{
			return (T*)GetUserContext(ResourceType::GetHash(typeid(T)));
		}

		static void AddUserContext(uint32_t hash, void* pUserContext);
		static void* GetUserContext(uint32_t hash);

		UUIDRemapper m_UUIDRemapper;

	private:
		void Initialize();

	private:
		GloryContext();
		virtual ~GloryContext();

	private:
		friend class Engine;
		friend class Object;
		static GloryContext* m_pContext;
		Game* m_Game;
		CameraManager m_CameraManager;
		AssetDatabase* m_pAssetDatabase;
		ResourceTypes* m_pResourceTypes;
		Serializers* m_pSerializers;
		DisplayManager* m_pDisplayManager;
		Console* m_pConsole;
		LayerManager* m_pLayerManager;
		ShaderManager* m_pShaderManager;
		Reflect* m_pReflection;
		ObjectManager* m_pObjectManager;
		Debug* m_pDebug;
		std::map<size_t, void*> m_pUserContexts;
	};
}