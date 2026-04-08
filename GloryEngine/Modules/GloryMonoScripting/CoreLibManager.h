#pragma once
#include "mono_visibility.h"

#include "IMonoLibManager.h"
#include "MonoScriptManager.h"

#include <UUID.h>
#include <mono/metadata/object-forward.h>

namespace Glory
{
    class MonoManager;

    class CoreLibManager : public IMonoLibManager
    {
    public:
        GLORY_MONO_API CoreLibManager(MonoManager* pMonoManager);
        GLORY_MONO_API Assembly* GetAssemblyBinding() const;
        GLORY_MONO_API MonoObject* GetScriptDummy(MonoClass* pClass);
        GLORY_MONO_API MonoObject* CreateScript(size_t typeIndex, UUID sceneID, UUID objectID, UUID componentID);
        GLORY_MONO_API MonoObject* GetScript(UUID sceneID, UUID objectID, UUID componentID);
        GLORY_MONO_API const MonoScriptManager& ScriptManager() const;
        GLORY_MONO_API MonoScriptManager& ScriptManager();
        GLORY_MONO_API MonoObject* GetEngine();

    public:
        virtual void CollectTypes(IEngine*, Assembly*) override;
        void Initialize(IEngine* pEngine, Assembly* pAssembly) override;
        void Cleanup(IEngine* pEngine) override;
        void Reset(IEngine* pEngine) override;

        MonoObject* CreateAssetObject(UUID uuid, const std::string_view type);
        MonoObject* CreateSceneObject(UUID objectID, UUID sceneID);

    private:
        void CreateEngine(IEngine* pEngine);

        void OnSceneDestroy(UUID sceneID);
        void OnSceneObjectDestroy(UUID objectID, UUID sceneID);

    private:
        MonoManager* m_pMonoManager;
        MonoScriptManager m_ScriptManager;
        Assembly* m_pAssembly;
        MonoObject* m_pEngineObject;
        MonoMethod* m_pEngineReset;
        uint32_t m_EngineGCHandle;

        UUID m_SceneClosingCallback;
        UUID m_SceneObjectDestroyedCallback;
    };
}
