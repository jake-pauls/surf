/// CODEGEN
#include "gen.h" 
/// CODEGEN

#include "Utils.h"
#include "Define.h"
#include "Server.h"

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/select.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include <caml/callback.h>

/// @brief Signal flag to ensure runtime stays running
static volatile sig_atomic_t s_IsRunning = 1;

/// @brief Catches SIGINT to ensure program stops if cancelled by an interruption
/// @param _ Signal code - unused
static void CatchInterruptionSignal(int _) 
{
    (void) _;
    s_IsRunning = 0;
}

/// @brief Retrieves the in-address for a passed socket
/// @param socketAddress Socket to retrieve the address of
/// @return Reference to the in-address for the passed socket
void* GetInAddress(struct sockaddr* socketAddress)
{
    // IPv4
	if (socketAddress->sa_family == AF_INET)
		return &(((struct sockaddr_in*) socketAddress)->sin_addr);

    // IPv6
	return &(((struct sockaddr_in6*) socketAddress)->sin6_addr);
}

int surf_OpenConnection(StaticEnvironment* env)
{
    char* title;
    int status;
    struct addrinfo pHints, *pAddrInfo;

    int serverSocket, clientSocket;
    int socketArr[] = { clientSocket, serverSocket };
    size_t socketArrLen = 2;

#ifdef WIN32
    title = "<><>- surf api. [winsock] -<><>\n";
    printf(title);

    WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) 
    {
		SURF_API_SERVER_LOG("WSAStartup failed with error: %d", iResult);
		return 1;
	}
#else 
    title = "<><>- surf api. [unix] -<><>\n";
    printf(title);
#endif

    // Clear hints before using them
    memset(&pHints, 0, sizeof(pHints));
    pHints.ai_family = AF_UNSPEC;
    pHints.ai_socktype = SOCK_STREAM;
    pHints.ai_protocol = IPPROTO_TCP;
    pHints.ai_flags = AI_PASSIVE;

    // Get address info
    status = getaddrinfo(NULL, API_PORT, &pHints, &pAddrInfo);
    if (status != 0)
        SURF_API_SERVER_LOG("getaddrinfo error: %d", status);

    // Open server socket
    serverSocket = socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
    if (serverSocket < 0)
        SURF_API_SERVER_LOG("socket error: %d", serverSocket);

    // Bind server socket
    status = bind(serverSocket, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen);
    if (status < 0)
        SURF_API_SERVER_LOG("bind error: %d", status);

    // Start listening
    status = listen(serverSocket, 10);
    if (status < 0)
        SURF_API_SERVER_LOG("listen error: %d", status);

    // Cleanup the address info once socket is opened
    freeaddrinfo(pAddrInfo);

    struct sockaddr_storage clientAddr;
    char sendBuffer[SURF_MAX_BUFFER_SIZE];
    socklen_t sendBufferLen = sizeof(clientAddr);

    char recvBuffer[SURF_MAX_BUFFER_SIZE];
    socklen_t recvBufferLen = SURF_MAX_BUFFER_SIZE;

    while (s_IsRunning) 
    {
        struct timeval timeout = { 1, 0 };
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(serverSocket, &fds);

        // Listen for sockets without blocking
        select(serverSocket + 1, &fds, NULL, NULL, &timeout);
        if (!FD_ISSET(serverSocket, &fds))
            continue;

        // Accept connections and return information to socket descriptor
        clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddr, &sendBufferLen);
        if (clientSocket < 0)
        {
            SURF_API_SERVER_LOG("accept error: %d", clientSocket);
            continue;
        }

        inet_ntop(clientAddr.ss_family, GetInAddress((struct sockaddr*) &clientAddr), sendBuffer, sizeof(sendBuffer));
        SURF_API_SERVER_LOG("new connection: %s", sendBuffer);

        // Receive data until the client disconnects
        int rBytes;
        do 
        {
            rBytes = recv(clientSocket, recvBuffer, recvBufferLen, 0);
            if (rBytes > 0)
            {
                char* fData = strndup(recvBuffer, rBytes);

                // Testing: Interpret data submitted
                char* res = surfgen_Interp(env, fData);
                SURF_API_SERVER_LOG("%s", res);
                
                // Temp: Add a newline for the client
                size_t resLen = strlen(res);
                char newLine = '\n';

                char* resNewLine = malloc(resLen + 1 + 1);
                strcpy(resNewLine, res);
                resNewLine[resLen] = newLine;
                resNewLine[resLen + 1] = '\0';

                // Testing: "Pong" data back to the user
                status = send(clientSocket, resNewLine, strlen(resNewLine), 0);

                // Free data 
                free(fData);
                free(resNewLine);
            }
        } while (rBytes > 0 && status >= 0);
        
        // Client disconnected unexpectedly, kill sockets
        if (status == -1)
        {
            surf_KillSockets(socketArr, socketArrLen);
            _Exit(4);
        }
    }

    // Default exit, kill sockets
    surf_KillSockets(socketArr, socketArrLen);

    return 0;
}

void surf_KillSockets(int socketArr[], size_t socketArrLen)
{
    for (size_t i = 0; i < socketArrLen; ++i)
    {
#ifdef WIN32
        closesocket(socketArr[i]);
#else
        close(socketArr[i]);
#endif
    }

#ifdef WIN32
    WSACleanup();
#endif
}

int surf_InitServer()
{
    // Initialize signal for interruptions (ie: server is accessed via command line)
    signal(SIGINT, CatchInterruptionSignal);

    // Initialize static environment
    StaticEnvironment* env = surfgen_InitRuntime();

    // Open API connections
    int res = surf_OpenConnection(env);
    return res;
}