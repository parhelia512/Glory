#pragma once

#ifdef GLORY_FSM_MONO_EXPORTS
// BUILD LIB
#define GLORY_FSM_MONO_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_FSM_MONO_API __declspec(dllimport)
#endif
