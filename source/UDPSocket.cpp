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

 int UDPSocket::readFromSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int timeoutMilli){
    int ans;
    fd_set rset; 
    socklen_t aLength;
    peerAddr.sin_family = AF_INET;
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 500000;
    
    // clear the descriptor set 
    FD_ZERO(&rset); 
    FD_SET(sock, &rset);
    // select the ready descriptor 
    int nready = select(sock, &rset, NULL, NULL, &tv);
    if (nready == 0) 
    {
      printf("Timeout occurred!\n");
    } 
    do {
        cout << peerAddr.sin_family<< endl; 
        if (FD_ISSET(sock, &rset)) {
            cout << peerAddr.sin_family<< endl; 
            ans = recvfrom(sock, buffer, maxBytes, 0, (struct sockaddr *)  &peerAddr, &aLength);
            cout << buffer[0] << endl;
        }
        
    
    } while (ans==-1);
    return ans;
    
 }