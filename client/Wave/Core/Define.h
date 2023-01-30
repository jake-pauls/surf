#pragma once

#include <filesystem>

#ifdef _WAVE_DEBUG
	#if defined(_WIN32)
		#define WAVE_DEBUGBREAK() __debugbreak()
	#else
		#include <signal.h>
		#define WAVE_DEBUGBREAK() raise(SIGTRAP)
	#endif
#else
	#define WAVE_DEBUGBREAK()
#endif

#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x

#define __FILE_NAME__ std::filesystem::path(__FILE__).filename().string()
#define __FILE_NAME_C_STR__ std::filesystem::path(__FILE__).filename().string().c_str()
