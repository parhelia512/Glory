#pragma once

#ifdef GLORY_LOCALIZE_EXPORTS
// BUILD LIB
#define GLORY_LOCALIZE_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_LOCALIZE_API __declspec(dllimport)
#endif
