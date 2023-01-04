#include "Log.h"

#ifdef _WAVE_ENABLE_LOGGING
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef _WIN32
#include <spdlog/sinks/msvc_sink.h>
#endif

namespace
{
	bool s_IsInitialized = false;
	std::shared_ptr<spdlog::logger> s_CoreLogger;
	std::string s_CoreLogPattern = "[%m-%d-%C %X.%e] %^[%l] %v%$";
}

void core::InitializeLogging()
{
	if (!s_IsInitialized)
	{
		s_IsInitialized = true;

		auto defaultSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		defaultSink->set_level(spdlog::level::debug);

		std::vector<spdlog::sink_ptr> coreSinks { defaultSink };

#ifdef _WIN32
		auto vsOutputSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
		vsOutputSink->set_level(spdlog::level::debug);
		coreSinks.push_back(vsOutputSink);
#endif

		s_CoreLogger = std::make_shared<spdlog::logger>("debug", coreSinks.begin(), coreSinks.end());
		s_CoreLogger->set_level(spdlog::level::debug);
		s_CoreLogger->set_pattern(s_CoreLogPattern);
		spdlog::set_default_logger(s_CoreLogger);
	}
}

void core::KillLogging()
{
	if (s_IsInitialized)
	{
		s_IsInitialized = false;
		s_CoreLogger->flush();
	}

	spdlog::shutdown();
}

void core::Log(const ELogType type, const char* message)
{
	if (!s_CoreLogger)
		return;

	WAVE_ASSERT(message && message[0], "Attempting to log an empty string");

	switch (type)
	{
	case ELogType::Debug:
		s_CoreLogger->debug(message);
		break;
	case ELogType::Info:
		s_CoreLogger->info(message);
		break;
	case ELogType::Warn:
		s_CoreLogger->warn(message);
		break;
	case ELogType::Error:
		s_CoreLogger->error(message);
		break;
	}
}

#endif