#pragma once
#include <string>
#include <vector>

namespace Glory
{
	class ScriptingExtender;
	class InternalCall;

	class IScriptExtender
	{
	public:
		virtual void GetInternalCalls(std::vector<InternalCall>& internalCalls) = 0;
		virtual void GetLibs(ScriptingExtender* pScriptingExtender) = 0;
	};
}
