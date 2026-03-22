#pragma once
#include <UUID.h>

#include <engine_visibility.h>

#include <unordered_map>

namespace Glory
{
	template<typename T>
	struct DistributedRandom;

	/**
	 * @brief Remapper for UUIDs
	 * 
	 * Uses a distributed seeded random to remap UUIDs to new values.
	 * Results are stored so requesting a remap for the same
	 * seed again will return the same result.
	 */
	struct UUIDRemapper
	{
	public:
		/* @brief Constructor that creates a passthrough remapper */
		GLORY_ENGINE_API explicit UUIDRemapper();
		/* @brief Constructor that initializes the random distributer with a seed */
		GLORY_ENGINE_API explicit UUIDRemapper(uint32_t seed);
		GLORY_ENGINE_API virtual ~UUIDRemapper();
		GLORY_ENGINE_API UUID operator()(const UUID uuid);
		GLORY_ENGINE_API bool Find(const UUID uuid, UUID& outUUID) const;

		GLORY_ENGINE_API void SoftReset(uint32_t seed);
		GLORY_ENGINE_API void SoftReset();
		GLORY_ENGINE_API void Reset();
		GLORY_ENGINE_API void Reset(uint32_t seed);
		GLORY_ENGINE_API void EnforceRemap(UUID one, UUID two);

	private:
		DistributedRandom<uint64_t>* m_pRandom;
		std::unordered_map<UUID, UUID> m_Results;
		bool m_IsPassThrough;
		uint32_t m_Seed;
	};
}
