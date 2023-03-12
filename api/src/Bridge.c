/// CODEGEN
#include "gen.h" 
/// CODEGEN

#include "surf/Utils.h"
#include "surf/Bridge.h"

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

/// @brief Port number the API runs on
#define API_PORT "3030"

/// @brief Signal flag to ensure runtime stays running
static volatile sig_atomic_t s_IsRunning = 1;

/// @brief Catches SIGINT to ensure program stops if cancelled by an interruption
/// @param _ Signal code - unused
static void CatchInterruptionSignal(int _) 
{
    (void) _;
    s_IsRunning = 0;
}

void* GetInAddress(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*) sa)->sin_addr);

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

int surf_OpenConnection(StaticEnvironment* env)
{
    char* title;
    int status;
    struct addrinfo pHints, *pAddrInfo;
    int serverSocket, clientSocket;

#ifdef WIN32
    title = "<><>- surf api. [winsock] -<><>\n";
    printf(title);

    WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) 
    {
		printf("WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}
#else 
    title = "<><>- surf api. [unix] -<><>\n";
    printf(title);
#endif

    // Clear hints before using them
    memset(&pHints, 0, sizeof pHints);
    pHints.ai_family = AF_UNSPEC;
    pHints.ai_socktype = SOCK_STREAM;
    pHints.ai_flags = AI_PASSIVE;

    // Get address info
    status = getaddrinfo(NULL, API_PORT, &pHints, &pAddrInfo);
    if (status != 0)
    {
        printf("getaddrinfo error: %d\n", status);
    }

    // Open server socket
    serverSocket = socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
    if (status < 0)
    {
        printf("socket error: %d\n", status);
    }

    // Bind server socket
    status = bind(serverSocket, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen);
    if (status < 0)
    {
        printf("bind error: %d\n", status);
    }

    // Start listening
    status = listen(serverSocket, 10);
    if (status < 0)
    {
        printf("listen error: %d\n", status);
    }

    // Cleanup the address info once socket is opened
    freeaddrinfo(pAddrInfo);

    struct sockaddr_storage clientAddr;
    char sendBuffer[INET6_ADDRSTRLEN];
    socklen_t sendBufferLen = sizeof clientAddr;

    char recvBuffer[INET6_ADDRSTRLEN];
    socklen_t recvBufferLen = INET6_ADDRSTRLEN;

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
            printf("accept error: %d\n", clientSocket);
            continue;
        }

        inet_ntop(clientAddr.ss_family, GetInAddress((struct sockaddr*) &clientAddr), sendBuffer, sizeof sendBuffer);
        printf("<><>- new connection: %s -<><>\n", sendBuffer);
        status = send(clientSocket, title, strlen(title), 0);

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
                printf("%s\n", res);
                
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
        
        // Client disconnected unexpectedly, cleanup sockets
        if (status == -1)
        {
#ifdef WIN32
            closesocket(clientSocket);
            closesocket(serverSocket);
            WSACleanup();
#else
            close(clientSocket);
            close(listenSocket);
#endif

            _Exit(4);
        }
    }

    // Cleanup sockets
#ifdef WIN32
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
#else
    close(clientSocket);
    close(listenSocket);
#endif

    return 0;
}

int surf_Init()
{
    // Initialize signal for interruptions (ie: server is accessed via command line)
    signal(SIGINT, CatchInterruptionSignal);

    // Initialize static environment
    StaticEnvironment* env = surfgen_InitRuntime();

    // Open API connections
    int res = surf_OpenConnection(env);
    return res;
}