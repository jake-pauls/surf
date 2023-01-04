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
