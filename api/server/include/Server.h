#ifndef Server_h
#define Server_h

#include <stddef.h>

/// @brief Initialization function for the surf server
/// @return Code indicating success or failure
int surf_InitServer();

/// @brief Performs platform specific cleanup for allocated sockets
/// @param socketArr Array containing allocated sockets
/// @param socketArrLen Length of the socket array
void surf_KillSockets(int socketArr[], size_t socketArrLen);

#endif /* Server_h */