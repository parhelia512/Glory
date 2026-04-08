#pragma once
#include "Version.h"

#define GLORY_CORE_VERSION_MAJOR 0
#define GLORY_CORE_VERSION_MINOR 6
#define GLORY_CORE_VERSION_SUBMINOR 0

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define GLORY_CORE_VERSION TOSTRING(GLORY_CORE_VERSION_MAJOR.GLORY_CORE_VERSION_MINOR.GLORY_CORE_VERSION_SUBMINOR)

namespace Glory
{
	constexpr std::string_view GloryCoreVersion = GLORY_CORE_VERSION;
	const Version CoreVersion = Version::Parse(GloryCoreVersion);
}
