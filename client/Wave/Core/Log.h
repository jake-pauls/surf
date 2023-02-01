#pragma once

#include <source_location>

#include <spdlog/spdlog.h>

/// @brief Logging types available for log messages
enum class ELogType
{
	Error,
	Warn,
	Info,
	Debug
};

namespace core
{
#ifdef _WAVE_ENABLE_LOGGING
	/// @brief Initializes logging sinks across the renderer
	void InitializeLogging();

	/// @brief Flushes logging sinks across the renderer
	void KillLogging();

	/// @brief Base logging function for a log message without arguments
	/// @param type Enum to determine the type of log being created
	/// @param message Message to insert into the log
	void Log(const ELogType type, const char* message);

	/// @brief Variadic logging function for a log message with arguments
	/// @param type Enum to determine the type of log being created
	/// @param message Message to insert into the log
	/// @param ...args Arguments to pass into the log, uses fmt formatting
	template<typename... TArgs>
	inline void Log(const ELogType type, const char* message, TArgs... args)
	{
		if constexpr (sizeof...(args) > 0)
		{
			// Parse variadics using fmt formatting
			// ex: core::Log(ELogType::Debug, "The meaning of life is {}", 42);
			fmt::format_args fmtArgs = fmt::make_format_args(args...);
			std::string fmtString = fmt::vformat(message, fmtArgs);
			core::Log(type, fmtString.c_str());
		}
		else
		{
			core::Log(type, message);
		}
	}

#else
	inline void InitializeLogging() { }
	inline void KillLogging() { }
	inline void Log (const ELogType type, const char* message) { }

	template<typename... TArgs>
	inline void Log(const ELogType type, const char* message, TArgs... args) { }
#endif
}
