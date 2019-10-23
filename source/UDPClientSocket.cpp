UDPClientSocket::UDPClientSocket (char *machine,  int port) : UDPSocket(machine,  port) {
    setBroadcast(sock);
    makeLocalSA(&myAddr);
    if( bind(sock, (struct sockaddr *) &myAddr, sizeof(struct sockaddr_in))!= 0){
        perror("Bind failed\n");
        close(sock);
        return;
    }
    makeDestSA(&peerAddr,machine, port);
    client = true;
}
bool UDPClientSocket::initializeClient (char * _peerAddr, int _peerPort){

}
UDPClientSocket::~UDPClientSocket ( ){
}
