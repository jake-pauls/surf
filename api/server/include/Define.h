#ifndef Define_h
#define Define_h

#define LOG_HELPER(fmt, ...) printf("[surf::api::server] " fmt "\n%s", __VA_ARGS__) 

/// @brief Variadic logging macro
#define SERVER_LOG(...) LOG_HELPER(__VA_ARGS__, "")

/// @brief Port to run the server on
#define API_PORT "3030"

#endif /* Define_h */