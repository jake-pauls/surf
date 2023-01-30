#pragma once

#include <vulkan/vulkan.h>

#include "Log.h"
#include "Define.h"

#ifdef _WAVE_ENABLE_LOGGING
	#define WAVE_ASSERT(check, message) { if (!(check)) { core::Log(ELogType::Error, "[{1}::{2}] {0}", message, __FILE_NAME__, __LINE__); WAVE_DEBUGBREAK(); } }
#else
	#define WAVE_ASSERT(check, message)
#endif

/// @brief Checks Vulkan API calls for returning non-zero error codes and logs them to the error channel
/// @param error The VkResult of the Vukan API call 
/// @param function The function call from which the error was caused
/// @param file String containing the name of the file from which the error was triggered
/// @param line Line number in the file from which the error was triggered 
/// @return True if no errors were logged to the console, assert will not trip
bool VkCheckError(VkResult error, const char* function, const char* file, int line);

#define VK_CALL(x) { VkResult error = x; \
	WAVE_ASSERT(VkCheckError(error, #x, __FILE_NAME_C_STR__, __LINE__), "VK_CALL faied, refer to Vulkan trace above for more information"); }