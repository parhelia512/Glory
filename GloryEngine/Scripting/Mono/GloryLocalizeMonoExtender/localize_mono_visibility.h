#pragma once

#ifdef GLORY_LOCALIZE_MONO_EXPORTS
// BUILD LIB
#define GLORY_LOCALIZE_MONO_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_LOCALIZE_MONO_API __declspec(dllimport)
#endif
