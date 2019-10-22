#include "../header/UDPSocket.h"


UDPSocket::UDPSocket(char *machine,  int port){
    if(( sock = socket(AF_INET, SOCK_DGRAM, 0))<0) {
        std::perror("socket failed");
        return;
    }
}

UDPSocket:: ~UDPSocket ( ){
    close(sock);
}



int UDPSocket:: writeToSocket (char * buffer, int maxBytes ){
    int aLength = sizeof(peerAddr);
    peerAddr.sin_family = AF_INET;

    return sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)  &peerAddr, sizeof(struct sockaddr_in));
}

int UDPSocket:: readFromSocketWithNoBlock (char * buffer, int maxBytes ){
    socklen_t aLength;
    peerAddr.sin_family = AF_INET;

    return recvfrom(sock, buffer, maxBytes, 0, (struct sockaddr *)  &peerAddr, &aLength);
}
