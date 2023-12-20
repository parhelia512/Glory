#include "JoltMonoExtender.h"
#include "PhysicsCSAPI.h"
#include "PhysicsComponentsCSAPI.h"

#include <GloryMonoScipting.h>
#include <ScriptingExtender.h>
#include <Engine.h>

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

	bool OnLoadExtra(Glory::GloryContext*, const char* path, Module* pModule, Module* pRequiredModule)
	{
		GloryMonoScipting* pScripting = (GloryMonoScipting*)pRequiredModule;
		IScriptExtender* pScriptExtender = new JoltMonoExtender(path);
		pScripting->GetScriptingExtender()->RegisterExtender(pScriptExtender);
		return true;
	}

	void JoltLibManager::Initialize(Assembly* pAssembly)
	{
		
	}

	void JoltLibManager::Cleanup()
	{
		
	}
}
