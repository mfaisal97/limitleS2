#ifndef SERVER_H
#define SERVER_H

#include "UDPServerSocket.h"
#include "UDPClientSocket.h"
#include "../header/Message.h"

class Server
{
    private:
        UDPServerSocket * udpServerSocket;
        Message * getRequest();
        Message * doOperation();
        void sendReply (Message * _message);
    public:
        Server(char * _listen_hostname, int _listen_port);
        void serveRequest();
        ~Server();
};

#include "../source/Server.cpp"
#endif // SERVER_H
