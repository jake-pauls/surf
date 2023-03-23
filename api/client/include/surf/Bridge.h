#ifndef Bridge_h
#define Bridge_h

#include <stddef.h>

/// @brief Result enum for operations over the surf API
typedef enum surf_ApiResult
{
	SURF_API_ERROR = -1,
	SURF_API_SUCCESS = 0,
} surf_ApiResult;

/// @brief Starts bridge between client and server surf API, will timeout if server is not found
/// @return surf API result indicating if the connection succeeded or not
surf_ApiResult surf_StartBridge();

/// @brief Kills sockets and destroys bridge between client and server surf API
/// @return surf API result indicating if destruction of the bridge was succesful or not
surf_ApiResult surf_DestroyBridge();

/// @brief INTERNAL: Performs platform specific sending of data through the API socket
/// @param buffer Constant string containing the data to submit to the server
/// @param bufferLen Length of the string buffer being sent
/// @param flags Flags for the MSDN 'send' or unix 'send' functions
/// @return Number of bytes send if successful, -1 if not
int surf_InternalSendSocket(const char* buffer, size_t bufferLen, int flags);

/// @brief INTERNAL: Performs platform specific receiving of data through the API socket, adds null-termination to passed buffer
/// @param outBuffer Pre-sized string buffer to receive data into
/// @param outBufferLen Length of the pre-sized string buffer
/// @param flags Flags for the MSDN 'recv' or unix 'recv' functions
/// @return Number of bytes received if succesful, -1 if not
int surf_InternalReceiveSocket(char* outBuffer, size_t outBufferLen, int flags);

/// @brief INTERNAL: Performs platform specific cleanup for an allocated socket
/// @param socket Socket to kill
/// @return surf API result indicating if cleaning up the socket succeeded
surf_ApiResult surf_InternalCloseSocket(int socket);

#endif /* Bridge_h */