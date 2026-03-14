#include "AudioModule.h"
#include "AudioComponents.h"
#include "AudioComponentManager.h"

#include <IEngine.h>
#include <SceneManager.h>

#include <EntityRegistry.h>
#include <EntityID.h>

namespace Glory
{
	AudioModule::AudioModule() {}
	AudioModule::~AudioModule() {}

	const std::type_info& AudioModule::GetBaseModuleType()
	{
		return typeid(AudioModule);
	}

	void AudioModule::Initialize()
	{
		Reflect::SetReflectInstance(&m_pEngine->Reflection());

		Reflect::RegisterEnum<SpatializationMode>();
		Reflect::RegisterEnum<AmbisonicsOrder>();
		Reflect::RegisterEnum<OcclusionType>();
		Reflect::RegisterEnum<AirAbsorptionType>();
		Reflect::RegisterType<SpatializationSettings>();
		Reflect::RegisterType<AttenuationSettings>();
		Reflect::RegisterType<AirAbsorptionSettings>();
		Reflect::RegisterType<DirectivitySettings>();
		Reflect::RegisterType<OcclusionSettings>();
		Reflect::RegisterType<TransmissionSettings>();
		Reflect::RegisterType<DirectSimulationSettings>();
		Reflect::RegisterType<ReflectionSimulationSettings>();
		Reflect::RegisterType<PathingSimulationSettings>();
		Reflect::RegisterType<AudioSourceSimulationSettings>();

		Reflect::RegisterType<AudioSimulationSettings>();

		m_pEngine->GetSceneManager()->RegisterComponentManager<AudioSourceManager, AudioSource>(
			[this](Utils::ECS::EntityRegistry*, AudioSourceManager* manager) {
				manager->m_pAudioModule = this;
				manager->m_pAssetManager = &m_pEngine->GetAssetManager();
			});
		m_pEngine->GetSceneManager()->RegisterComponentManager<AudioListenerManager, AudioListener>(
			[this](Utils::ECS::EntityRegistry*, AudioListenerManager* manager) {
				manager->m_pAudioModule = this;
			});
	}

	void AudioModule::LoadSettings(ModuleSettings& settings)
	{
		settings.RegisterValue<unsigned int>(SettingNames::MixingChannels.data(), 64);
		settings.RegisterValue<unsigned int>(SettingNames::SamplingRate.data(), 48000);
		settings.RegisterValue<unsigned int>(SettingNames::Framesize.data(), 1024);
	}
}