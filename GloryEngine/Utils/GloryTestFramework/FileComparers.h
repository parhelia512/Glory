#pragma once
#include "TestComparators.h"

#include <filesystem>

namespace Glory::Utils
{
	/** @brief Compare 2 binary files */
	struct CompareBinaryFiles
	{
		std::expected<std::string, std::string> operator()(const std::filesystem::path& a, const std::filesystem::path& b);

	private:
		void Overwrite(const std::filesystem::path& a, const std::filesystem::path& b);
	};
}
