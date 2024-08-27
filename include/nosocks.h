#ifndef NOSOCKS_H_
#define NOSOCKS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#ifdef _WIN32
#ifdef NOSOCKS_EXPORTS
#define NOSOCKS_API __declspec(dllexport)
#else
#define NOSOCKS_API __declspec(dllimport)
#endif
#else
#define NOSOCKS_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int iResult;
  WSADATA wsaData;
  SOCKET clientSocket;
  struct addrinfo *result;
  struct addrinfo hints;
} ServerInst;

typedef struct {
  int iResult;
  WSADATA wsaData;
  SOCKET clientSocket;
  struct addrinfo *ptr;
  struct addrinfo *result;
  struct addrinfo hints;
} ClientInst;

NOSOCKS_API int serverInst(ServerInst *sI);
NOSOCKS_API int clientInst(ClientInst *cI, const char *ipv4_addr);

#ifdef __cplusplus
}
#endif

#endif // NOSOCKS_H_
