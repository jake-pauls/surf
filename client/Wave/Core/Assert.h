#pragma once

#include "Log.h"
#include "Define.h"

#ifdef _WAVE_ENABLE_LOGGING
	#define WAVE_ASSERT(check, message) { if (!(check)) { core::Log(ELogType::Error, "[{1}::{2}] {0}", message, __FILE_NAME__, __LINE__); WAVE_DEBUGBREAK(); } }
#else
	#define WAVE_ASSERT(check, message)
#endif