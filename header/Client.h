#ifndef CLIENT_H
#define CLIENT_H

#include "UDPClientSocket.h"
#include "Message.h"

class Client
{
    private:
        UDPClientSocket * udpSocket;
    public:
        Client(char * _hostname, int _port);
        Message * execute(Message * _message);
        ~Client();
};

#include "../source/Client.cpp"
#endif // CLIENT_H
