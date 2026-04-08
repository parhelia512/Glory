#pragma once

#ifdef GLORY_JOLT_EXPORTS
// BUILD LIB
#define GLORY_JOLT_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_JOLT_API __declspec(dllimport)
#endif
