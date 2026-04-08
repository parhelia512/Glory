#pragma once

#ifdef GLORY_OVERLAY_CONSOLE_EXPORTS
// BUILD LIB
#define GLORY_OVERLAY_CONSOLE_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_OVERLAY_CONSOLE_API __declspec(dllimport)
#endif
