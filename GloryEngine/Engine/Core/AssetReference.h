#pragma once
#include "UUID.h"
#include "ResourceType.h"
#include "Engine.h"
#include "Resources.h"

#include <Reflection.h>

namespace Glory
{
	class Resource;

	class AssetReferenceBase
	{
	public:
		AssetReferenceBase();
		AssetReferenceBase(UUID uuid);
		virtual ~AssetReferenceBase();

		const UUID AssetUUID() const;
		UUID* AssetUUIDMember();
		void SetUUID(UUID uuid);

		virtual const uint32_t TypeHash() { return 0; };

		virtual AssetReferenceBase* CreateCopy() { return new AssetReferenceBase(m_AssetUUID); };

	protected:
		REFLECTABLE(AssetReferenceBase, (UUID) (m_AssetUUID))
	};

	template<class T>
	class AssetReference : public AssetReferenceBase
	{
	public:
		AssetReference() : AssetReferenceBase() {}
		AssetReference(UUID uuid) : AssetReferenceBase(uuid) {}
		AssetReference(T* pAsset) : AssetReferenceBase(pAsset ? pAsset->GetUUID() : 0) {}
		virtual ~AssetReference() {}

		virtual const uint32_t TypeHash() override
		{
			static const uint32_t typeHash = ResourceType::GetHash<T>();
			return typeHash;
		}

		AssetReferenceBase* CreateCopy() override
		{
			return new AssetReference<T>(m_AssetUUID);
		}

		T* Get(Engine* pEngine) const
		{
			T* pResource = pEngine->GetResources().Manager<T>()->Get(m_AssetUUID);
			return pResource ? dynamic_cast<T*>(pResource) : nullptr;
		}
	};
}
