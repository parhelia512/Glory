#include "JoltMonoExtender.h"
#include "PhysicsCSAPI.h"
#include "PhysicsComponentsCSAPI.h"
#include "MonoScriptedManager.h"

#include <GloryMonoScipting.h>
#include <JoltPhysicsModule.h>
#include <ScriptingExtender.h>
#include <IEngine.h>

namespace Glory
{
	JoltMonoExtender::JoltMonoExtender(const char* path) : m_pLibManager(new JoltLibManager()), m_Path(path)
	{
	}

	JoltMonoExtender::~JoltMonoExtender()
	{
		delete m_pLibManager;
		m_pLibManager = nullptr;
	}

	void JoltMonoExtender::GetInternalCalls(std::vector<InternalCall>& internalCalls)
	{
		PhysicsCSAPI::AddInternalCalls(internalCalls);
		PhysicsComponentsCSAPI::AddInternalCalls(internalCalls);
	}

	void JoltMonoExtender::GetLibs(ScriptingExtender* pScriptingExtender)
	{
		pScriptingExtender->AddInternalLib(m_Path, "GloryEngine.Jolt.dll", m_pLibManager);
	}

	bool OnLoadExtra(const char* path, Module* pModule, Module* pRequiredModule)
	{
		GloryMonoScipting* pScripting = (GloryMonoScipting*)pRequiredModule;
		IScriptExtender* pScriptExtender = new JoltMonoExtender(path);
		pScripting->GetScriptingExtender()->RegisterExtender(pScriptExtender);
		return true;
	}

	void JoltLibManager::CollectTypes(IEngine*, Assembly*)
	{
	}

	void JoltLibManager::Initialize(IEngine* pEngine, Assembly*)
	{
		PhysicsCSAPI::SetEngine(pEngine);
		PhysicsComponentsCSAPI::SetEngine(pEngine);

		JoltPhysicsModule* pJoltPhysics = pEngine->GetModule<JoltPhysicsModule>();
		pJoltPhysics->OnBodyActivated_Callback = MonoScriptedManager::OnBodyActivated;
		pJoltPhysics->OnBodyDeactivated_Callback = MonoScriptedManager::OnBodyDeactivated;
		pJoltPhysics->OnContactAdded_Callback = MonoScriptedManager::OnContactAdded;
		pJoltPhysics->OnContactPersisted_Callback = MonoScriptedManager::OnContactPersisted;
		pJoltPhysics->OnContactRemoved_Callback = MonoScriptedManager::OnContactRemoved;
	}

	void JoltLibManager::Cleanup(IEngine* pEngine)
	{
		JoltPhysicsModule* pJoltPhysics = pEngine->GetModule<JoltPhysicsModule>();
		pJoltPhysics->OnBodyActivated_Callback = NULL;
		pJoltPhysics->OnBodyDeactivated_Callback = NULL;
		pJoltPhysics->OnContactAdded_Callback = NULL;
		pJoltPhysics->OnContactPersisted_Callback = NULL;
		pJoltPhysics->OnContactRemoved_Callback = NULL;
	}

	void JoltLibManager::Reset(IEngine*)
	{
	}
}
