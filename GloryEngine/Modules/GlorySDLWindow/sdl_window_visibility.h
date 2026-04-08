#pragma once

#ifdef GLORY_SDL_WINDOW_EXPORTS
// BUILD LIB
#define GLORY_SDL_WINDOW_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_SDL_WINDOW_API __declspec(dllimport)
#endif
