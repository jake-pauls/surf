#ifndef Define_h
#define Define_h

#include "surf/Utils.h"

#include <stdio.h>
#include <string.h>

#ifdef WIN32
	/// @brief Platform-safe strdup
	#define STRDUP(s) _strdup(s)
	/// @brief Platform-safe strtok
	#define STRTOK(s, d, c) strtok_s(s, d, c)
	/// @brief Platform-safe asprintf
	#define ASPRINTF(p, f, ...) vasprintf(p, f, __VA_ARGS__)
#else
	/// @brief Platform-safe strdup
	#define STRDUP(s) strdup(s)
	/// @brief Platform-safe strtok
	#define STRTOK(s, d, c) strtok_r(s, d, c)
	/// @brief Platform-safe asprintf
	#define ASPRINTF(p, f, ...) asprintf(p, f, __VA_ARGS__)
#endif

/// @brief Client varidaic log helper to append newlines and tag the log
#define SURF_API_CLIENT_LOG_HELPER(fmt, ...) printf("[surf::api::client] " fmt "\n%s", __VA_ARGS__) 

/// @brief Client variadic logging macro
#define SURF_API_CLIENT_LOG(...) SURF_API_CLIENT_LOG_HELPER(__VA_ARGS__, "")

/// @brief Server port number - this should match the number the surf server exists on
#define SURF_API_PORT "3030"

/// @brief Transmission/reading buffer size 
/// @todo Write more wrappers to determine buffer sizes (ie: don't rely on this)
#define SURF_MAX_BUFFER_SIZE 256

/// @brief Wrapper for 'true'
#define SURF_TRUE 1

/// @brief Wrapper for 'false'
#define SURF_FALSE 0

#endif /* Define_h */