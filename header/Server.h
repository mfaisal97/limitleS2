#ifndef SERVER_H
#define SERVER_H

#include "UDPServerSocket.h"
#include "UDPClientSocket.h"
#include "Message.h"

class Server
{
    private:
        Message * getRequest();
        virtual Message * doOperation(Message * message);
        void sendReply (Message * _message);
    protected:
        UDPServerSocket * udpServerSocket;
        int getMyPort(){
          return udpServerSocket->getMyPort();
        }
    public:
        Server(char * _listen_hostname, int _listen_port);
        void serveRequest();
        ~Server();
};

#include "../source/Server.cpp"
#endif // SERVER_H
