#include "headerConfig.c"
#include "customDataTypes.h"

typedef struct customAddInfo sendingInfo;
void handleErrorSend(char* msg, sendingInfo* info) {
    info->status = -1;
    strcpy(info->message, msg);
    return;
}
sendingInfo sendUDP(char* msg, int msgLength, char* port, char* ip) {

    sendingInfo returnMessage;
#ifdef _WIN32
    WSADATA wsaData;
    // Initialize Winsock2.2
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        handleErrorSend("WSAStartup failed", &returnMessage);
        return returnMessage;
    }
#endif

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1) {
        handleErrorSend("Can't create a socket", &returnMessage);
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

    int status = sendto(sockfd, msg, msgLength, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if (status == -1) {
        handleErrorSend("Failure to send message", &returnMessage);
        closesocket(sockfd);
#ifdef _WIN32
        WSACleanup();
#endif
        return returnMessage;
    }

    closesocket(sockfd);
    returnMessage.status = 0;
    char* returnStatus;
    strcpy(returnMessage.message, "Message sent successfully");
#ifdef _WIN32
    WSACleanup();
#endif
    return returnMessage;
}