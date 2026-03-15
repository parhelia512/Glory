#include "FileComparers.h"
#include "Tester.h"

#include <fstream>

namespace Glory::Utils
{
    std::expected<std::string, std::string> CompareBinaryFiles::operator()(const std::filesystem::path& a, const std::filesystem::path& b)
    {
        if (!std::filesystem::exists(a))
            throw std::exception(std::format("Input file {} does not exist.", a.string()).data());
        if (!std::filesystem::exists(b))
        {
            Overwrite(a, b);
            throw std::exception(std::format("Expected file {} does not exist.", b.string()).data());
        }

        std::ifstream fileA(a, std::fstream::in | std::ios::binary | std::ios::ate);
        std::ifstream fileB(b, std::fstream::in | std::ios::binary | std::ios::ate);

        const size_t sizeA = fileA.tellg();
        const size_t sizeB = fileB.tellg();
        if (sizeA != sizeB)
        {
            fileA.close();
            fileB.close();
            Overwrite(a, b);
            return std::unexpected(std::format("Expected the size of {} to equal {} but there is a size mismatch: {} bytes vs {} bytes.",
                a.string(), b.string(), sizeA, sizeB));
        }

        fileA.seekg(0, std::ios_base::beg);
        fileB.seekg(0, std::ios_base::beg);

        while (!fileA.eof() && !fileB.eof())
        {
            char charA;
            char charB;
            fileA >> charA;
            fileB >> charB;

            if (charA != charB)
            {
                fileA.close();
                fileB.close();
                Overwrite(a, b);
                return std::unexpected(std::format("Byte at {} mismatch, expected {} to equal {}.",
                    size_t(fileA.tellg()), charA, charB));
            }
        }
        return std::format("{} == {}", a.string(), b.string());
    }

    void CompareBinaryFiles::Overwrite(const std::filesystem::path& a, const std::filesystem::path& b)
    {
        if (!Tester::Instance()->ShouldSaveFiles()) return;
        std::filesystem::copy(a, b, std::filesystem::copy_options::overwrite_existing);
    }
}
