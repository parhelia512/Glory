#pragma once
#include <cstdint>
#include <vector>
#include <Glory.h>

namespace Glory
{
	class InternalCall;
	class IEngine;
	class GScene;

	GLORY_API uint32_t GetComponentHash(const std::string& name);
	GLORY_API GScene* GetEntityScene(uint64_t sceneID);

	class EntityCSAPI
	{
	public:
		static void GetInternallCalls(std::vector<InternalCall>& internalCalls);
		static void SetEngine(IEngine* pEngine);
	};
}
