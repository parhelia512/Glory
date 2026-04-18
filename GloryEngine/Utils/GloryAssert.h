#pragma once
#include <string_view>
#include <iostream>

namespace Glory::Utils
{
    template<typename T>
    inline void AssertImpl(T&& assertion, const std::string_view message,
        const std::string_view file, const int line, std::ostream& out = std::cerr)
    {
        if (assertion) return;
        out << file << '(' << line << "): *** assertion failed: " << message << '\n';
        throw;
    }
}

#define GLORY_ASSERT(assertion, message) do{ Glory::Utils::AssertImpl(assertion, message, __FILE__, __LINE__); } while(false);

#ifdef _DEBUG
#define GLORY_DEBUG_ASSERT(assertion, message) do{ Glory::Utils::AssertImpl(assertion, message, __FILE__, __LINE__); } while(false);
#else
#define GLORY_DEBUG_ASSERT(assertion, message)
#endif