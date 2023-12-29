#include "GloryMonoScipting.h"
#include "CoreCSAPI.h"
#include "MathCSAPI.h"
#include "InputCSAPI.h"
#include "EntityCSAPI.h"
#include "MonoManager.h"
#include "ScriptedComponentSerializer.h"
#include "MonoComponents.h"
#include "MonoScriptedSystem.h"
#include "ScriptingExtender.h"
#include "MonoScriptLoader.h"

#include <SceneManager.h>
#include <PropertySerializer.h>
#include <Engine.h>

namespace Glory
{
	GLORY_MODULE_VERSION_CPP(GloryMonoScipting);

	GloryMonoScipting::GloryMonoScipting() : m_pMonoManager(new MonoManager(this)), m_pScriptingExtender(new ScriptingExtender())
	{
	}

	GloryMonoScipting::~GloryMonoScipting()
	{
		delete m_pMonoManager;
		m_pMonoManager = nullptr;
	}

	MonoManager* GloryMonoScipting::GetMonoManager() const
	{
		return m_pMonoManager;
	}

	const std::type_info& GloryMonoScipting::GetModuleType()
	{
		return typeid(GloryMonoScipting);
	}

	ScriptingExtender* GloryMonoScipting::GetScriptingExtender()
	{
		return m_pScriptingExtender;
	}

	void GloryMonoScipting::LoadSettings(ModuleSettings& settings)
	{
		settings.PushGroup("Debugging");
		settings.RegisterValue<bool>("Enable Debugging", false);
		settings.RegisterEnumValue<MonoLogLevel>("MonoLogLevel", MonoLogLevel::error);
		settings.RegisterValue<std::string>("MonoDebuggingIP", "127.0.0.1");
		settings.RegisterValue<uint32_t>("MonoDebuggingPort", 55555);

		settings.PushGroup("Garbage Collection");
		settings.RegisterValue<bool>("Enable GC allocation logging", false);
		settings.RegisterValue<bool>("Auto Collect Garbage", true);
	}

	void GloryMonoScipting::Initialize()
	{
		m_pMonoManager->Initialize("./Modules/GloryMonoScripting/Dependencies");
		m_pScriptingExtender->Initialize(this);

		Reflect::RegisterEnum<MonoLogLevel>();

		SceneManager* pScenes = m_pEngine->GetSceneManager();
		Glory::Utils::ECS::ComponentTypes* pComponentTypes = pScenes->ComponentTypesInstance();
		pScenes->RegisterComponent<MonoScriptComponent>();
		PropertySerializer::RegisterSerializer<ScriptedComponentSerailizer>();

		ResourceType::RegisterResource<MonoScript>("");

		// Scripted
		pComponentTypes->RegisterInvokaction<MonoScriptComponent>(Glory::Utils::ECS::InvocationType::OnAdd, MonoScriptedSystem::OnAdd);
		pComponentTypes->RegisterInvokaction<MonoScriptComponent>(Glory::Utils::ECS::InvocationType::Update, MonoScriptedSystem::OnUpdate);
		pComponentTypes->RegisterInvokaction<MonoScriptComponent>(Glory::Utils::ECS::InvocationType::Draw, MonoScriptedSystem::OnDraw);
		pComponentTypes->RegisterInvokaction<MonoScriptComponent>(Glory::Utils::ECS::InvocationType::Start, MonoScriptedSystem::OnStart);
		pComponentTypes->RegisterInvokaction<MonoScriptComponent>(Glory::Utils::ECS::InvocationType::Stop, MonoScriptedSystem::OnStop);
		pComponentTypes->RegisterInvokaction<MonoScriptComponent>(Glory::Utils::ECS::InvocationType::OnValidate, MonoScriptedSystem::OnValidate);

		m_pEngine->AddLoaderModule(new MonoScriptLoader());
	}

	void GloryMonoScipting::PostInitialize()
	{
		m_pMonoManager->InitialLoad();
	}

	void GloryMonoScipting::Cleanup()
	{
		m_pMonoManager->Cleanup();
	}

	void GloryMonoScipting::AddLib(const ScriptingLib& library)
	{
		m_pMonoManager->AddLib(library);
	}

	void GloryMonoScipting::Bind(const InternalCall& internalCall)
	{
		const std::string& scriptMethod = internalCall.ScriptMethod();
		mono_add_internal_call(scriptMethod.c_str(), internalCall.RedirectMethod());
	}

	void GloryMonoScipting::GetInternalCalls(std::vector<InternalCall>& internalCalls)
	{
		CoreCSAPI::AddInternalCalls(internalCalls);
		MathCSAPI::AddInternalCalls(internalCalls);
		InputCSAPI::AddInternalCalls(internalCalls);
		EntityCSAPI::GetInternallCalls(internalCalls);
	}

	void GloryMonoScipting::GetLibs(ScriptingExtender* pScriptingExtender)
	{
		std::filesystem::path path = GetMetaData().Path().parent_path();
		path.append("Scripting/csharp");
		pScriptingExtender->AddInternalLib(path.string(), "GloryEngine.Core.dll", m_pMonoManager->GetCoreLibManager());
	}
}
