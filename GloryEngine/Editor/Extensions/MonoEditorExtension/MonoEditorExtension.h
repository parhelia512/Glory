#pragma once
#include <BaseEditorExtension.h>
#include <ProjectSpace.h>
#include <ObjectMenu.h>
#include <ResourceMeta.h>
#include <IPlayModeHandler.h>
#include <efsw/efsw.hpp>

#ifdef GLORY_MONO_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_MONO_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_MONO_EXTENSION_API __declspec(dllimport)
#endif

EXTENSION_H(GLORY_MONO_EXTENSION_API)

namespace Glory
{
    class GloryMonoScipting;
}

namespace Glory::Editor
{
    struct AssetCallbackData;

    class MonoEditorExtension : public BaseEditorExtension, public IPlayModeHandler, public efsw::FileWatchListener
    {
    public:
        virtual const char* ModuleName() override { return "Mono Scripting"; };
        virtual void HandleBeforeStart(Module* pModule) override {};
        virtual void HandleStart(Module* pModule) override {};
        virtual void HandleStop(Module* pModule) override;
        virtual void HandleUpdate(Module* pModule) override {};

        virtual void OnBeginPackage(const std::filesystem::path& path) override;
        virtual void OnGenerateConfigExec(std::ofstream& stream) override;
        virtual void OnEndPackage(const std::filesystem::path& path) override;
        virtual void handleFileAction(efsw::WatchID watchid, const std::string& dir,
            const std::string& filename, efsw::Action action,
            std::string oldFilename = "") override;

    public:
        MonoEditorExtension();
        virtual ~MonoEditorExtension();

        static void OpenCSharpProject();

        static void OpenFile(const std::filesystem::path& path);

        static size_t CompilationVersion();

    private:
        virtual void Initialize() override;
        virtual void Update() override;
        static void FindVisualStudioPath();

        void OnProjectClose(ProjectSpace* pProject);
        void OnProjectOpen(ProjectSpace* pProject);

        static void OnCreateScript(Object* pObject, const ObjectMenuType& menuType);
        static void OnOpenCSharpProject(Object* pObject, const ObjectMenuType& menuType);

        static void CopyEngineAssemblies(ProjectSpace* pProject);
        static void GeneratePremakeFile(ProjectSpace* pProject);
        static void GenerateBatchFile(ProjectSpace* pProject);
        static void RunGenerateProjectFilesBatch(ProjectSpace* pProject);

        static void CompileProject(ProjectSpace* pProject, bool release=false);
        static void ReloadAssembly(ProjectSpace* pProject);

        static void AssetCallback(const AssetCallbackData& callback);

        void Preferences();

    private:
        static GloryMonoScipting* m_pMonoScriptingModule;
        static size_t m_CompilationCounter;
    };
}