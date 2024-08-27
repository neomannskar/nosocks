#include "../include/nosocks.h"

#define DEFAULT_PORT "27015"

// Server

NOSOCKS_API int serverInst(ServerInst *sI) {
  if (sI == NULL)
    return 1;

  // Initialize Winsock
  sI->iResult = WSAStartup(MAKEWORD(2, 2), &(sI->wsaData));
  if (sI->iResult != 0) {
    printf("WSAStartup failed with error: %d\n", sI->iResult);
    return 1;
  }

  ZeroMemory(&(sI->hints), sizeof(sI->hints));
  sI->hints.ai_family = AF_INET;
  sI->hints.ai_socktype = SOCK_STREAM;
  sI->hints.ai_protocol = IPPROTO_TCP;
  sI->hints.ai_flags = AI_PASSIVE;

  sI->iResult = getaddrinfo(NULL, DEFAULT_PORT, &(sI->hints), &(sI->result));
  if (sI->iResult != 0) {
    printf("getaddrinfo failed with error: %d\n", sI->iResult);
    WSACleanup();
    return 1;
  }

  SOCKET tempSocket;

  tempSocket = socket(sI->result->ai_family, sI->result->ai_socktype,
                      sI->result->ai_protocol);
  if (tempSocket == INVALID_SOCKET) {
    printf("socket failed with error: %d\n", WSAGetLastError());
    freeaddrinfo(sI->result);
    WSACleanup();
    return 1;
  }

  sI->iResult =
      bind(tempSocket, sI->result->ai_addr, (int)sI->result->ai_addrlen);
  if (sI->iResult == SOCKET_ERROR) {
    printf("bind failed with error: %d\n", WSAGetLastError());
    freeaddrinfo(sI->result);
    closesocket(tempSocket);
    WSACleanup();
    return 1;
  }

  freeaddrinfo(sI->result);

  sI->iResult = listen(tempSocket, SOMAXCONN);
  if (sI->iResult == SOCKET_ERROR) {
    printf("listen failed with error: %d\n", WSAGetLastError());
    closesocket(tempSocket);
    WSACleanup();
    return 1;
  }

  sI->clientSocket = accept(tempSocket, NULL, NULL);
  if (sI->clientSocket == INVALID_SOCKET) {
    printf("accept failed with error: %d\n", WSAGetLastError());
    closesocket(tempSocket);
    WSACleanup();
    return 1;
  }

  closesocket(tempSocket);
  return 0;
}

// Client

NOSOCKS_API int clientInst(ClientInst *cI, const char *ipv4_addr) {
  if (cI == NULL || ipv4_addr == NULL)
    return 1;

  cI->clientSocket = INVALID_SOCKET;
  cI->result = NULL;
  cI->ptr = NULL;

  // Initialize Winsock
  cI->iResult = WSAStartup(MAKEWORD(2, 2), &(cI->wsaData));
  if (cI->iResult != 0) {
    printf("WSAStartup failed with error: %d\n", cI->iResult);
    return 1;
  }

  ZeroMemory(&(cI->hints), sizeof(cI->hints));
  cI->hints.ai_family = AF_UNSPEC;
  cI->hints.ai_socktype = SOCK_STREAM;
  cI->hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  cI->iResult =
      getaddrinfo(ipv4_addr, DEFAULT_PORT, &(cI->hints), &(cI->result));
  if (cI->iResult != 0) {
    printf("getaddrinfo failed with error: %d\n", cI->iResult);
    WSACleanup();
    return 1;
  }

  // Attempt to connect to an address until one succeeds
  for (cI->ptr = cI->result; cI->ptr != NULL; cI->ptr = cI->ptr->ai_next) {
    // Create a SOCKET for connecting to server
    cI->clientSocket =
        socket(cI->ptr->ai_family, cI->ptr->ai_socktype, cI->ptr->ai_protocol);
    if (cI->clientSocket == INVALID_SOCKET) {
      printf("socket failed with error: %d\n", WSAGetLastError());
      WSACleanup();
      return 1;
    }

    // Connect to server.
    cI->iResult =
        connect(cI->clientSocket, cI->ptr->ai_addr, (int)cI->ptr->ai_addrlen);
    if (cI->iResult == SOCKET_ERROR) {
      closesocket(cI->clientSocket);
      cI->clientSocket = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(cI->result);

  if (cI->clientSocket == INVALID_SOCKET) {
    printf("Unable to connect to server!\n");
    WSACleanup();
    return 1;
  }

  return 0;
}
