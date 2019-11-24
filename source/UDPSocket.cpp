#include "../header/UDPSocket.h"

UDPSocket::UDPSocket(char *machine,  int port){
    if(( sock = socket(AF_INET, SOCK_DGRAM, 0))<0) {
        std::perror("socket failed");
        return;
    }
}

UDPSocket::~UDPSocket ( ){
    close(sock);
}



int UDPSocket::writeToSocket (char * buffer, int maxBytes ){
    int aLength = sizeof(peerAddr);
    peerAddr.sin_family = AF_INET;

    return sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)  &peerAddr, sizeof(struct sockaddr_in));
}

int UDPSocket::readFromSocketWithNoBlock (char * buffer, int maxBytes ){
    socklen_t aLength;
    peerAddr.sin_family = AF_INET;

    return recvfrom(sock, buffer, maxBytes, 0, (struct sockaddr *)  &peerAddr, &aLength);
}

int UDPSocket::readFromSocketWithBlock (char * buffer, int maxBytes ){
    socklen_t aLength;
    peerAddr.sin_family = AF_INET;
    int ans;
    do {
        ans = recvfrom(sock, buffer, maxBytes, 0, (struct sockaddr *)  &peerAddr, &aLength);
    } while (ans==-1);
    return ans;
}

sockaddr_in UDPSocket::getmyAddr(){
    return myAddr;
}
sockaddr_in UDPSocket::getPeerAddr(){
    return peerAddr;
}

 int UDPSocket::readFromSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int timeoutMilli){
    int ans;
    socklen_t aLength;
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    sockaddr_in* recvaddr;
    if (client){
        recvaddr = new sockaddr_in;
    }{
        recvaddr = &peerAddr;
    }

    // select the ready descriptor
    int ret = select(sock+1, &readfds, NULL, NULL, &tv);
    if (ret > 0)
    {
      if (recvfrom(sock, buffer, maxBytes, 0, (struct sockaddr *)  recvaddr, &aLength) >= 0)
        {
            // todo: verify the packet is an acknowledgement
            // of the packet sent above and not something else...
            std::cout << "ack received" << std::endl;
        }
        else
        {
            //cout << "error reading" << endl;
        }
    }
    else if (ret == 0)
    {
        std::cout << "timed out waiting for ack" << std::endl;
        // todo: resend the same packet again, or abort the transfer
    }
    else
    {
        //cout << "error selecting" << endl;
    }
    //do {
        //cout << peerAddr.sin_family<< endl;
        //if (FD_ISSET(sock, &rset)) {
            //cout << peerAddr.sin_family<< endl;
            // ans = recvfrom(sock, buffer, maxBytes, 0, (struct sockaddr *)  &peerAddr, &aLength);
            //cout << buffer[0] << endl;
        //}


    //} while (ans==-1);
    return 1;

 }
