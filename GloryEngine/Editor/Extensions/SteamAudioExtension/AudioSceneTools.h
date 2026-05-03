#pragma once
#include "steam_audio_extension_visibility.h"

#include <AudioScene.h>

namespace Glory
{
	class GScene;
	class IEngine;
}

namespace Glory::Editor
{
	GLORY_STEAM_AUDIO_EXTENSION_API bool GenerateAudioScene(IEngine* pEngine, GScene* pScene,
		const SoundMaterial* defaultMaterial, AudioScene& audioScene);
}
