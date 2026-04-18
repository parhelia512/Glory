#pragma once
#include <string_view>
#include <iostream>

namespace Glory::Utils
{
    /** @brief Assert implementation
     * @param assertion Assertion
     * @param message Message to log when assertion fails.
     * @param file File where the assertion failed.
     * @param line Line number in the file where the assertion failed.
     * @param out Output stream.
     */
    template<typename T>
    inline void AssertImpl(T&& assertion, const std::string_view message,
        const std::string_view file, const int line, std::ostream& out=std::cerr)
    {
        if (assertion) return;
        out << file << '(' << line << "): *** assertion failed: " << message << '\n';
        throw;
    }
}

/** @brief Custom assert
 * @param assertion Assertion
 * @param message Message to log when assertion fails.
 */
#define GLORY_ASSERT(assertion, message) do{ Glory::Utils::AssertImpl(assertion, message, __FILE__, __LINE__); } while(false);

#ifdef _DEBUG
/** @brief Custom assert for debug builds
 * @param assertion Assertion
 * @param message Message to log when assertion fails.
 */
#define GLORY_DEBUG_ASSERT(assertion, message) do{ Glory::Utils::AssertImpl(assertion, message, __FILE__, __LINE__); } while(false);
#else
#define GLORY_DEBUG_ASSERT(assertion, message)
#endif
