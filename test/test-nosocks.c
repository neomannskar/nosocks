#include "../include/nosocks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>


#define SERVER_PORT "27015"
#define TEST_ADDRESS "127.0.0.1"

void test_server() {
  ServerInst server;
  int result;

  // Initialize server
  result = serverInst(&server);
  if (result != 0) {
    printf("Server initialization failed\n");
    return;
  }

  printf("Server initialized and listening on port %s\n", SERVER_PORT);

  // Accept a connection (block until client connects)
  SOCKET clientSocket = server.clientSocket;
  if (clientSocket == INVALID_SOCKET) {
    printf("Server accept failed\n");
    WSACleanup();
    return;
  }

  printf("Client connected\n");

  // Cleanup
  closesocket(clientSocket);
  WSACleanup();
}

void test_client() {
  ClientInst client;
  int result;

  // Initialize client
  result = clientInst(&client, TEST_ADDRESS);
  if (result != 0) {
    printf("Client initialization failed\n");
    return;
  }

  printf("Client connected to server at %s:%s\n", TEST_ADDRESS, SERVER_PORT);

  // Cleanup
  closesocket(client.clientSocket);
  WSACleanup();
}

int main() {
  printf("Starting test...\n");

  // Run server and client tests
  test_server();
  Sleep(1000); // Wait for the server to be ready
  test_client();

  printf("Test completed.\n");
  return 0;
}