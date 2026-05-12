#pragma once
#include "StringTable.h"

#include <AssetReference.h>

#include <Reflection.h>

namespace Glory
{
	struct StringTableRef
	{
		StringTableRef() : m_STReference(0ull) {}
		StringTableRef(UUID uuid) : m_STReference(uuid) {}

		bool operator==(const StringTableRef&& other)
		{
			return other.m_STReference.GetUUID() == other.m_STReference.GetUUID();
		}

		bool operator==(StringTableRef& other)
		{
			return other.m_STReference.GetUUID() == other.m_STReference.GetUUID();
		}

		REFLECTABLE(StringTableRef,
			(ResourceReference<StringTable>) (m_STReference)
		)
	};

	/** @brief String table loader component */
	struct StringTableLoader
	{
		StringTableLoader() : m_StringTable(0ull), m_KeepLoaded(false) {}

		REFLECTABLE(StringTableLoader,
			(ResourceReference<StringTable>)(m_StringTable),
			(bool)(m_KeepLoaded)
		);
	};
	
	/** @brief Text localizer component */
	struct Localize
	{
		REFLECTABLE(Localize,
			(std::string) (m_Term)
		);
	};
}
