#pragma once

#ifdef GLORY_STEAM_AUDIO_EXTENSION_EXPORTS
// BUILD LIB
#define GLORY_STEAM_AUDIO_EXTENSION_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_STEAM_AUDIO_EXTENSION_API __declspec(dllimport)
#endif