#pragma once

#include <string>

namespace core
{
    /// @brief Utilities and helpers for working with strings
    class StringHelpers
    {
    public:
        /// @brief Checks if two strings are equal
        /// @param a First string to check
        /// @param b Second string to check against
        /// @return True if both strings are equal
        static bool Equals(const std::string& a, const std::string& b);
    };
}