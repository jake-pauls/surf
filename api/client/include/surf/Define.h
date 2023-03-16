#ifndef Define_h
#define Define_h

#define LOG_HELPER(fmt, ...) printf("[surf::api::client] " fmt "\n%s", __VA_ARGS__) 

/// @brief Variadic logging macro
#define CLIENT_LOG(...) LOG_HELPER(__VA_ARGS__, "")

/// @brief Server port number - this should match the number the surf server exists on
#define API_PORT "3030"

#endif /* Define_h */