#include "surf/Bridge.h"
#include "surf/Define.h"
#include "surf/HashTable.h"
#include "surf/Interp.h"

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

#include <stdio.h>

/// @brief PID for profiling 
int g_APIPid;

/// @brief Single socket used to connect to the API
static int s_ApiSocket = -1;

/// @brief Boolean representing the connection status of the bridge
static int s_IsBridgeConnected = SURF_FALSE;

surf_ApiResult surf_StartBridge()
{
    SURF_PROFILE_INIT();
    SURF_PROFILE_START("surf", "api runtime", g_APIPid);
    SURF_PROFILE_START_STEP("initialization", "connection");

    int status;
    struct addrinfo pHints, *pAddrInfo, *pNextAddr;
 
#ifdef WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        SURF_API_CLIENT_LOG("client WSAStartup failed with error: %d\n", iResult);
        return SURF_API_ERROR;
    }
#endif

    memset(&pHints, 0, sizeof(pHints));
    pHints.ai_family = AF_UNSPEC;
    pHints.ai_socktype = SOCK_STREAM;
    pHints.ai_protocol = IPPROTO_TCP;

    // Assume that the surf API will always connect over localhost
    status = getaddrinfo("localhost", SURF_API_PORT, &pHints, &pAddrInfo);
    if (status != 0)
    {
        SURF_API_CLIENT_LOG("client getaddrinfo error: %d\n", status);
        surf_DestroyBridge();
        return SURF_API_ERROR;
    }

    int attempts = 0;

    // Passing "localhost" returns all available local addresses, iterate to find the right one
    for (pNextAddr = pAddrInfo; pAddrInfo != NULL; pNextAddr = pAddrInfo->ai_next)
    {
        // Bail out if more than five addresses were tried, should be enough in most cases
        if (attempts > SURF_BRIDGE_MAX_CONNECTION_ATTEMPTS)
            break;

        attempts++;

        s_ApiSocket = socket(pNextAddr->ai_family, pNextAddr->ai_socktype, pNextAddr->ai_protocol);
        if (s_ApiSocket < 0)
        {
            SURF_API_CLIENT_LOG("client socket error: %d\n", s_ApiSocket);
            surf_DestroyBridge();
            return SURF_API_ERROR;
        }

        // Try to connect, continue and reconnect the socket if invalid
        status = connect(s_ApiSocket, pNextAddr->ai_addr, (int) pNextAddr->ai_addrlen);
        if (status < 0)
        {
            surf_InternalCloseSocket(s_ApiSocket);
            s_ApiSocket = -1;
            continue;
        }

        // Break if connection was successful
        s_IsBridgeConnected = SURF_TRUE;
        break;
    }

    // Free address info once connected
    freeaddrinfo(pAddrInfo);

    SURF_PROFILE_END_STEP("initialization", "connection");
    return s_IsBridgeConnected ? SURF_API_SUCCESS : SURF_API_ERROR;
}

surf_ApiResult surf_DestroyBridge()
{
    SURF_PROFILE_START_STEP("destruction", "disconnection");

    int result = SURF_API_SUCCESS;

    // TODO: Depending on this to reassign to SURF_API_ERROR is kind of unsafe...
    result = surf_InternalCloseSocket(s_ApiSocket);

    // Destroy items if any were registered in the interpreter's tables
    surf_InternalInterpDestroy();

#ifdef WIN32
    result = WSACleanup();
#endif

    SURF_PROFILE_END_STEP("destruction", "disconnection");
    SURF_PROFILE_END("surf", "api runtime", g_APIPid);
    SURF_PROFILE_DESTROY();
    return result;
}

int surf_InternalSendSocket(const char* buffer, size_t bufferLen, int flags)
{
    SURF_PROFILE_START_STEP("socket", "sending/receiving data from server");

    SURF_PROFILE_START_STEP("socket", "sending data to the server");
    int res = send(s_ApiSocket, buffer, bufferLen, flags);
    SURF_PROFILE_END_STEP("socket", "sending data to the server");

    return res;
}

int surf_InternalReceiveSocket(char* buffer, size_t bufferLen, int flags)
{
    SURF_PROFILE_START_STEP("socket", "receiving a response from the server");
    int bytes = recv(s_ApiSocket, buffer, bufferLen, flags);
    SURF_PROFILE_END_STEP("socket", "receiving a response from the server");

    SURF_PROFILE_END_STEP("socket", "sending/receiving data from server");

    // Add null-termination to string
    buffer[bytes - 1] = '\0';

    return bytes;
}

surf_ApiResult surf_InternalCloseSocket(int socket)
{
    SURF_PROFILE_START_STEP("socket", "closing socket");
    surf_ApiResult result = SURF_API_SUCCESS;

    if (socket != -1)
#ifdef WIN32
        result = closesocket(socket);
#else
        result = close(socket);
#endif

    SURF_PROFILE_END_STEP("socket", "closing socket");
    return result;
}

int surf_InternalIsBridgeConnected()
{
    return s_IsBridgeConnected;
}
