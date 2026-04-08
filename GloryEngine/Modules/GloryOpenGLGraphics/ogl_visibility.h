#pragma once

#ifdef GLORY_OGL_EXPORTS
// BUILD LIB
#define GLORY_OGL_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_OGL_API __declspec(dllimport)
#endif
