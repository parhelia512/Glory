#pragma once
#include "mono_visibility.h"

#include <string>
#include <cstdint>
#include <vector>

namespace Glory
{
	class InternalCall;
	class IEngine;
	class GScene;

	GLORY_MONO_API uint32_t GetComponentHash(const std::string& name);
	GLORY_MONO_API GScene* GetEntityScene(uint64_t sceneID);

	class EntityCSAPI
	{
	public:
		static void GetInternallCalls(std::vector<InternalCall>& internalCalls);
		static void SetEngine(IEngine* pEngine);
	};
}
