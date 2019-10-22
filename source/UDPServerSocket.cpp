#include "../header/UDPServerSocket.h"
UDPServerSocket::UDPServerSocket (char *machine,  int port):UDPSocket(machine,  port){ 

    makeReceiverSA(&myAddr, myPort);

    if( bind(sock,(struct sockaddr *) &myAddr, sizeof(struct sockaddr_in))!= 0){
        perror("Bind failed\n");
        close(sock);
        return;
    }

}

bool UDPServerSocket::initializeServer (char * _myAddr, int _myPort){

}

UDPServerSocket::~UDPServerSocket ( ){

}
