#pragma once
#include "Resource.h"

namespace Glory
{
	class AudioData : public Resource
	{
	public:
		AudioData();
		AudioData(std::vector<char>&& data);
		virtual ~AudioData() = default;

		const char* Data() const;
		const size_t Size() const;

		void Serialize(Utils::BinaryStream& container) const override;
		void Deserialize(Utils::BinaryStream& container) override;

	private:
		virtual void References(IEngine*, std::vector<UUID>&) const override {}

	private:
		std::vector<char> m_Data;
	};
}
