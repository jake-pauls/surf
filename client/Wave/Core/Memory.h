#pragma once

#include <memory>

namespace core
{
	/// @brief Wrapper declaration for unique pointers 
	/// @tparam T Type associated with the unique pointer 
	template<typename T>
	using Unique = std::unique_ptr<T>;

	/// @brief Constructs a unique pointer and forwards references passed to the pointer's associated constructor
	/// @tparam T Type associated with the unique pointer
	/// @tparam ...TArgs Type of arguments passed to the object's constructor
	/// @param ...args Arguments required to construct the pointer
	/// @return Constructed unique pointer
	template<typename T, typename... TArgs>
	constexpr Unique<T> CreateUnique(TArgs... args)
	{
		return std::make_unique<T>(std::forward<TArgs>(args)...);
	}

	/// @brief Wrapper declaration for shared pointers
	/// @tparam T Type associated with the shared pointer
	template<typename T>
	using Shared = std::shared_ptr<T>;

	/// @brief Constructs a shared pointer and forwards references passed to the pointer's associated constructor
	/// @tparam T Type associated with the shared pointer
	/// @tparam ...TArgs Type of arguments passed to the object's constructor
	/// @param ...args Arguments required to construct the pointer
	/// @return Constructed shared pointer
	template<typename T, typename... TArgs>
	constexpr Shared<T> CreateShared(TArgs... args)
	{
		return std::make_shared<T>(std::forward<TArgs>(args)...);
	}
}