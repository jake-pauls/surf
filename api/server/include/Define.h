#ifndef Define_h
#define Define_h

/// @brief Server varidaic log helper to append newlines and tag the log
#define SURF_API_SERVER_LOG_HELPER(fmt, ...) printf("[surf::api::server] " fmt "\n%s", __VA_ARGS__) 

/// @brief Server variadic logging macro
#define SURF_API_SERVER_LOG(...) SURF_API_SERVER_LOG_HELPER(__VA_ARGS__, "")

/// @brief Port to run the server on
#define API_PORT "3030"

/// @brief Transmission/reading buffer size 
/// @todo Write more wrappers to determine buffer sizes (ie: don't rely on this)
#define SURF_MAX_BUFFER_SIZE 256

#endif /* Define_h */