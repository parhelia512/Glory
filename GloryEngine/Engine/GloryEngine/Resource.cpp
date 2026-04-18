#include "Resource.h"
#include "Debug.h"

#include <BinaryStream.h>

namespace Glory
{
	Resource::Resource(): m_IsDirty(true)
	{
		APPEND_TYPE(Resource);
	}

	Resource::Resource(UUID uuid) : Object(uuid), m_IsDirty(true)
	{
		APPEND_TYPE(Resource);
	}

	Resource::Resource(const std::string& name) : Object(name), m_IsDirty(true)
	{
		APPEND_TYPE(Resource);
	}

	Resource::Resource(UUID uuid, const std::string& name) : Object(uuid, name), m_IsDirty(true)
	{
		APPEND_TYPE(Resource);
	}

	Resource::Resource(Resource&& other) noexcept: Object(), m_IsDirty(true)
	{
		m_Inheritence = std::move(other.m_Inheritence);
		m_ID = other.m_ID;
		m_Name = std::move(other.m_Name);
	}

	Resource& Resource::operator=(Resource&& other) noexcept
	{
		m_Inheritence = std::move(other.m_Inheritence);
		m_ID = other.m_ID;
		m_Name = std::move(other.m_Name);
		m_IsDirty = true;
		return *this;
	}

	Resource::~Resource()
	{
	}

	bool Resource::IsDirty() const
	{
		return m_IsDirty;
	}

	void Resource::SetDirty(bool dirty)
	{
		m_IsDirty = dirty;
	}

	void Resource::SetResourceUUID(UUID uuid)
	{
		m_ID = uuid;
	}
}
