#include "../include/UDPListen.h"
#include "../include/customDataTypes.h"
#include "../src/headerConfig.c"

/**
 * Function: getclientSocket
 * ---------------
 * Takes in the ip address of the server and create a TCP socket connecting to
 * that server and returns that socket once the connection has been established
 *
 * Parameters:
 * char* ip (ip address of the server we wish to connect)
 *
 * Returns:
 *  returns the pointer of socket info
 *  if the status is 0 it means that the socket for the connection is received
 *  if the status is -1 it means something went wrong.
 *
 * Side Effects:
 *  NONE
 *
 * Constraints:
 *  it has to be a valid ip address to the server
 *  the returned type is a pointer to an int.
 *
 * Example Usage:
 *  int *clientSocket = getClientSocket("127.0.0.1");
 *
 * Notes:
 *  check out the client.c file in archive for further insight
 *  check out customDataTypes
 */

int *getClientSocket(char *ip) {
  int *clientSocket = (int *)malloc(sizeof(int));

  struct sockaddr_in server;
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_port = htons(12345);

  *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    perror("[-] Socket could not be created\n");
    return NULL;
  }

  if (setsockopt(*clientSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1},
                 sizeof(int)) < 0) {
    printf("[-] Error while setting socket reusable");
    close(*clientSocket);
    free(clientSocket);
    return NULL;
  }

  printf("[+] Socket created successfully\n");

  struct customAddInfo message = listenUDP("12345", ip);
  if (message.status == 0) {
    sleep(3);
  }

  if (connect(*clientSocket, (struct sockaddr *)&server, sizeof(server)) != 0) {
    perror("[-] Connection failed\n");
    return NULL;
  }

  printf("[+] Connected to the server\n");
  printf("\n");

  return clientSocket;
}
