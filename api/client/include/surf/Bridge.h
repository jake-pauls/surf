#ifndef Bridge_h
#define Bridge_h

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Result code for operations over the surf API
typedef enum
{
	SURF_API_ERROR = -1,
	SURF_API_SUCCESS = 0,
} surf_ApiResult;

/// @brief Starts bridge between client and server surf API
/// @return Result code indicating if the connection succeeded or not
surf_ApiResult surf_StartBridge();

/// @brief Directly submits a line of code to the interpreter through the API, all strings returned must be freed 
/// @param line String buffer containing the line of code to submit to the interpreter
/// @return String buffer containing the interpreted result
char* surf_Interp(const char* line);

/// @brief Kills sockets and destroys bridge between client and server surf API
/// @return Positive result code indicating if destruction of the bridge was succesful or not, -1 if an error occurred
surf_ApiResult surf_DestroyBridge();

/// @brief INTERNAL USE: Performs platform specific cleanup for an allocated socket
/// @param socket Socket to kill
/// @return Positive result code indicating if cleaning up the socket succeeded, -1 if an error occurred
surf_ApiResult surf_CloseSocket(int socket);

#ifdef __cplusplus
}
#endif

#endif /* Bridge_h */