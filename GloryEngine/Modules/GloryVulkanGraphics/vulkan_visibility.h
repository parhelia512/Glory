#pragma once

#ifdef GLORY_VULKAN_EXPORTS
// BUILD LIB
#define GLORY_VULKAN_API __declspec(dllexport)
#else
// USE LIB
#define GLORY_VULKAN_API __declspec(dllimport)
#endif
