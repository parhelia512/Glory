#pragma once

#ifdef GLORY_ENGINE_STATIC
#define GLORY_ENGINE_API
#else
#ifdef GLORY_ENGINE_EXPORTS
// BUILD LIB
#define GLORY_ENGINE_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_ENGINE_API __declspec(dllimport)
#endif
#endif