#pragma once
#include "ResourceType.h"

#include <engine_visibility.h>

#include <UUID.h>

#include <Reflection.h>

namespace Glory
{
	class Resource;
	class AssetManager;

	class AssetReferenceBase
	{
	public:
		GLORY_ENGINE_API AssetReferenceBase();
		GLORY_ENGINE_API AssetReferenceBase(UUID uuid);
		GLORY_ENGINE_API virtual ~AssetReferenceBase();

		GLORY_ENGINE_API const UUID AssetUUID() const;
		GLORY_ENGINE_API UUID* AssetUUIDMember();
		GLORY_ENGINE_API void SetUUID(UUID uuid);
		GLORY_ENGINE_API Resource* GetResource(AssetManager* pAssets) const;
		GLORY_ENGINE_API Resource* GetResourceImmediate(AssetManager* pAssets) const;

		virtual const uint32_t TypeHash() { return 0; };

		virtual AssetReferenceBase* CreateCopy() { return new AssetReferenceBase(m_AssetUUID); };

		GLORY_ENGINE_API operator bool() const;

	protected:
		REFLECTABLE(AssetReferenceBase, (UUID) (m_AssetUUID))
	};

	template<class T>
	class AssetReference : public AssetReferenceBase
	{
	public:
		AssetReference() : AssetReferenceBase() {}
		AssetReference(UUID uuid) : AssetReferenceBase(uuid) {}
		AssetReference(uint64_t id) : AssetReferenceBase(id) {}
		AssetReference(T* pAsset) : AssetReferenceBase(pAsset ? pAsset->GetUUID() : UUID(0ull)) {}
		virtual ~AssetReference() {}

		virtual const uint32_t TypeHash() override
		{
			static const uint32_t typeHash = ResourceTypes::GetHash<T>();
			return typeHash;
		}

		AssetReferenceBase* CreateCopy() override
		{
			return new AssetReference<T>(m_AssetUUID);
		}

		T* Get(AssetManager* pManager) const
		{
			Resource* pResource = GetResource(pManager);
			return pResource ? dynamic_cast<T*>(pResource) : nullptr;
		}

		T* GetImmediate(AssetManager* pManager) const
		{
			Resource* pResource = GetResourceImmediate(pManager);
			return pResource ? dynamic_cast<T*>(pResource) : nullptr;
		}
	};
}
