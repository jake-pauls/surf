#pragma once

#include <functional>

namespace core
{
	/// @brief Fun little hash function
	template <typename T, typename... TArgs>
	inline void HashCombine(std::size_t& seed, const T& v, const TArgs&... targs) 
	{
		seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		(HashCombine(seed, targs), ...);
	};
}
