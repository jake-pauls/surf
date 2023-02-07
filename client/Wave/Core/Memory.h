#pragma once

#include <memory>

namespace core
{
	/// @brief Wrapper declaration for unique pointers 
	/// @tparam T Type associated with the unique pointer 
	template<typename T>
	using Unique = std::unique_ptr<T>;

	/// @brief Constructs a unique pointer and forwards references passed to its associated constructor
	/// @tparam T Type associated with the unique pointer
	/// @tparam ...TArgs Template argument representing the arguments in the constructor
	/// @param ...args Arguments required to construct the pointer
	/// @return Constructed unique pointer
	template<typename T, typename... TArgs>
	constexpr Unique<T> CreateUnique(TArgs... args)
	{
		return std::make_unique<T>(std::forward<TArgs>(args)...);
	}
}