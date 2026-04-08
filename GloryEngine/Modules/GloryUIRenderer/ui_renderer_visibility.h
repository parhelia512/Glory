#pragma once

#ifdef GLORY_UI_RENDERER_EXPORTS
// BUILD LIB
#define GLORY_UI_RENDERER_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_UI_RENDERER_API __declspec(dllimport)
#endif
