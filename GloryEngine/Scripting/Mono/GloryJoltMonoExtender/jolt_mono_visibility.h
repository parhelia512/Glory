#pragma once

#ifdef GLORY_JOLT_MONO_EXPORTS
// BUILD LIB
#define GLORY_JOLT_MONO_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_JOLT_MONO_API __declspec(dllimport)
#endif
