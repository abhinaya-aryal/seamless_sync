#include "headerConfig.c"
#include "../include/customDataTypes.h"
#include "../include/UDPErrorHandle.h"

struct customAddInfo sendUDP(char* msg, int msgLength, char* port, char* ip, int multipleSends) {
    struct customAddInfo returnMessage;
#ifdef _WIN32
    WSADATA wsaData;
    // Initialize Winsock2.2
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        handleUDPError("WSAStartup failed", &returnMessage);
        return returnMessage;
    }
#endif

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1) {
        handleUDPError("Can't create a socket", &returnMessage);
#ifdef _WIN32
        WSACleanup();
#endif
        return returnMessage;
    }

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(atoi(port));
    dest_addr.sin_addr.s_addr = inet_addr(ip);

    int status;
    // allowing for multiple sends over the same port and socket
    do {
        status = sendto(sockfd, msg, msgLength, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        if (status == -1) {
            handleUDPError("Failure to send message", &returnMessage);
#ifdef _WIN32
            WSACleanup();
            closesocket(sockfd);
#elif __linux__
            close(sockfd);
#endif
            return returnMessage;
        }
    } while (multipleSends);

#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#elif __linux__
    close(sockfd);
#endif
    // only incase of single send
    returnMessage.status = 0;
    strcpy(returnMessage.message, "Message sent successfully");
    return returnMessage;
}