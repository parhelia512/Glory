#pragma once

#ifdef GLORY_SDL_INPUT_EXPORTS
// BUILD LIB
#define GLORY_SDL_INPUT_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_SDL_INPUT_API __declspec(dllimport)
#endif
