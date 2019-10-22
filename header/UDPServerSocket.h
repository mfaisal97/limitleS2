#ifndef UDPSERVERSOCKET_H
#define UDPSERVERSOCKET_H
#include "../header/UDPSocket.h"
class UDPServerSocket : public UDPSocket
{
public: 
    UDPServerSocket (char *machine,  int port);
    bool initializeServer (char * _myAddr, int _myPort);
    ~UDPServerSocket ( );
};

#include "../source/UDPServerSocket.cpp"
#endif // UDPSERVERSOCKET_H
