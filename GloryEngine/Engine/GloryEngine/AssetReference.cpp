#include "AssetReference.h"
#include "AssetManager.h"

#include "ResourceReferencing.h"

namespace Glory
{
	AssetReferenceBase::AssetReferenceBase() : m_AssetUUID(0)
	{
	}

	AssetReferenceBase::AssetReferenceBase(UUID uuid) : m_AssetUUID(uuid)
	{
		if (uuid)
			AddResourceReference(uuid);
	}

	AssetReferenceBase::AssetReferenceBase(const AssetReferenceBase& other): m_AssetUUID(other.m_AssetUUID)
	{
		if (m_AssetUUID)
			AddResourceReference(m_AssetUUID);
	}

	AssetReferenceBase& AssetReferenceBase::operator=(const AssetReferenceBase& other)
	{
		m_AssetUUID = other.m_AssetUUID;
		if (m_AssetUUID)
			AddResourceReference(m_AssetUUID);
		return *this;
	}

	AssetReferenceBase::AssetReferenceBase(AssetReferenceBase&& other) noexcept: m_AssetUUID(other.m_AssetUUID)
	{
		other.m_AssetUUID = 0;
	}

	AssetReferenceBase& AssetReferenceBase::operator=(AssetReferenceBase&& other) noexcept
	{
		m_AssetUUID = other.m_AssetUUID;
		other.m_AssetUUID = 0;
		return *this;
	}

	AssetReferenceBase::~AssetReferenceBase()
	{
		if (m_AssetUUID)
			RemoveResourceReference(m_AssetUUID);
		m_AssetUUID = 0;
	}

	const UUID AssetReferenceBase::AssetUUID() const
	{
		return m_AssetUUID;
	}

	UUID* AssetReferenceBase::AssetUUIDMember()
	{
		return &m_AssetUUID;
	}

	void AssetReferenceBase::SetUUID(UUID uuid)
	{
		if (m_AssetUUID)
			RemoveResourceReference(m_AssetUUID);
		m_AssetUUID = uuid;
		if (uuid)
			AddResourceReference(uuid);
	}

	Resource* AssetReferenceBase::GetResource(AssetManager* pAssets) const
	{
		return pAssets->GetOrLoadAsset(m_AssetUUID);
	}

	Resource* AssetReferenceBase::GetResourceImmediate(AssetManager* pAssets) const
	{
		return pAssets->GetAssetImmediate(m_AssetUUID);
	}
	
	AssetReferenceBase::operator bool() const
	{
		return m_AssetUUID;
	}
}
