#pragma once
#include "Version.h"

#include <engine_visibility.h>

#include <vector>
#include <BitSet.h>

namespace Glory::Utils
{
	class BinaryStream;
}

namespace Glory
{
	class Resource;
	class IEngine;

	enum AssetArchiveFlags
	{
		None = 0,
		Read = 1 << 0,
		Write = 1 << 1,
		WriteVersion = 1 << 2,
		WriteNew = Write | WriteVersion,
	};

	class AssetArchive
	{
	public:
		GLORY_ENGINE_API AssetArchive(Utils::BinaryStream* pStream, AssetArchiveFlags flags=AssetArchiveFlags::Read);
		GLORY_ENGINE_API AssetArchive(AssetArchive&& other) noexcept;
		GLORY_ENGINE_API virtual ~AssetArchive();

		GLORY_ENGINE_API bool VerifyVersion();

		GLORY_ENGINE_API void Serialize(Resource* pResource) const;
		GLORY_ENGINE_API void Serialize(const Resource* pResource) const;
		GLORY_ENGINE_API void Deserialize(IEngine* pEngine);

		GLORY_ENGINE_API size_t Size() const;
		GLORY_ENGINE_API Resource* Get(IEngine* pEngine, size_t index) const;

	private:
		void WriteVersion();
		void ReadVersion();

		Resource* ReadResource(IEngine* pEngine);

		Utils::BinaryStream* m_pStream;
		Version m_Version;
		std::vector<Resource*> m_pResources;
		mutable Utils::BitSet m_Owned;
	};
}
