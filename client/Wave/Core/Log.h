#pragma once

#include <spdlog/spdlog.h>

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
	void InitializeLogging();

	void KillLogging();

	void Log(const ELogType type, const char* message);

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
