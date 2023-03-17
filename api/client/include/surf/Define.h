#ifndef Define_h
#define Define_h

#include <stdio.h>
#include <string.h>

#ifdef WIN32
	/// @brief Platform-safe strdup
	#define STRDUP(s) _strdup(s)
	/// @brief Platform-safe strtok
	#define STRTOK(s, d, c) strtok_s(s, d, c)
#else
	/// @brief Platform-safe strdup
	#define STRDUP(s) strdup(s)
	/// @brief Platform-safe strtok
	#define STRTOK(s, d, c) strtok_r(s, d, c)
#endif

/// @brief Client varidaic log helper to append newlines and tag the log
#define SURF_API_CLIENT_LOG_HELPER(fmt, ...) printf("[surf::api::client] " fmt "\n%s", __VA_ARGS__) 

/// @brief Client variadic logging macro
#define SURF_API_CLIENT_LOG(...) SURF_API_CLIENT_LOG_HELPER(__VA_ARGS__, "")

/// @brief Server port number - this should match the number the surf server exists on
#define SURF_API_PORT "3030"

/// @brief Transmission buffer size
#define SURF_MAX_BUFFER_SIZE 56

#endif /* Define_h */