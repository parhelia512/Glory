#pragma once
#include "Resource.h"

#include <engine_visibility.h>

namespace Glory
{
	class AudioData : public Resource
	{
	public:
		GLORY_ENGINE_API AudioData();
		GLORY_ENGINE_API AudioData(std::vector<char>&& data);
		virtual ~AudioData() = default;

		GLORY_ENGINE_API const char* Data() const;
		GLORY_ENGINE_API const size_t Size() const;

		GLORY_ENGINE_API void Serialize(Utils::BinaryStream& container) const override;
		GLORY_ENGINE_API void Deserialize(Utils::BinaryStream& container) override;

	private:
		virtual void References(IEngine*, std::vector<UUID>&) const override {}

	private:
		std::vector<char> m_Data;
	};
}
