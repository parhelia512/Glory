#pragma once

#ifdef GLORY_FSM_EXPORTS
// BUILD LIB
#define GLORY_FSM_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_FSM_API __declspec(dllimport)
#endif
