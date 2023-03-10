/// CODEGEN
#include "gen.h" 
/// CODEGEN

#include "surf/Utils.h"
#include "surf/Bridge.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <caml/callback.h>

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

void* GetInAddr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*) sa)->sin_addr);

	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

int surf_OpenConnection()
{
    int status;
    struct addrinfo pHints, *pAddrInfo;
    int listenSocket;

#ifdef WIN32
    printf("surf. API [WinSock]\n");

    WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) 
    {
		printf("WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}
#endif

    // Clear hints before using them
    memset(&pHints, 0, sizeof pHints);
    pHints.ai_family = AF_UNSPEC;
    pHints.ai_socktype = SOCK_STREAM;
    pHints.ai_flags = AI_PASSIVE;

    // Get address info
    status = getaddrinfo(NULL, "8888", &pHints, &pAddrInfo);
    if (status != 0)
    {
        printf("getaddrinfo error: %d\n", status);
    }

    listenSocket = socket(pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol);
    if (status < 0)
    {
        printf("socket error: %d\n", status);
    }

    status = bind(listenSocket, pAddrInfo->ai_addr, pAddrInfo->ai_addrlen);
    if (status < 0)
    {
        printf("bind error: %d\n", status);
    }

    status = listen(listenSocket, 10);
    if (status < 0)
    {
        printf("listen error: %d\n", status);
    }

    // Cleanup the address info once socket is opened
    freeaddrinfo(pAddrInfo);

    int clientSocket;
    struct sockaddr_storage clientAddr;
    char sendBuffer[INET6_ADDRSTRLEN];
    socklen_t sendBufferLen = sizeof clientAddr;

    char recvBuffer[INET6_ADDRSTRLEN];
    socklen_t recvBufferLen = INET6_ADDRSTRLEN;

    printf("Opening connections...\n");
    while (1) 
    {
        // Accept connections and return information to socket descriptor
        clientSocket = accept(listenSocket, (struct sockaddr*) &clientAddr, &sendBufferLen);
        if (clientSocket < 0)
        {
            printf("accept error: %d\n", clientSocket);
            continue;
        }

        inet_ntop(clientAddr.ss_family, GetInAddr((struct sockaddr*) &clientAddr), sendBuffer, sizeof sendBuffer);
        printf("Connected to %s\n", sendBuffer);
        status = send(clientSocket, "surf. API\n", 11, 0);

        // Receive data until the client disconnects
        int rBytes;
        do 
        {
            rBytes = recv(clientSocket, recvBuffer, recvBufferLen, 0);
            if (rBytes > 0)
            {
                char* data = strndup(recvBuffer, rBytes);

                // Testing: Interpret data submitted
                char* res = interp(data);
                printf("%s\n", res);

                // Testing: "Pong" data back to the user
                size_t resLen = strlen(res);
                status = send(clientSocket, res, resLen, 0);

                free(data);
            }
        } while (rBytes > 0 && status >= 0);
        
        // Client disconnected unexpectedly, cleanup sockets
        if (status == -1)
        {
#ifdef WIN32
            closesocket(clientSocket);
            closesocket(listenSocket);
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
    closesocket(listenSocket);
    WSACleanup();
#else
    close(clientSocket);
    close(listenSocket);
#endif

    return 0;
}

int surf_Init()
{
    // Opening API
    int res = surf_OpenConnection();
    return res;
}