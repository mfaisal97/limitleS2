#ifndef UDPCLIENTSOCKET_H
#define UDPCLIENTSOCKET_H

#include "UDPSocket.h"
#include "../common/utilities.h"

class UDPClientSocket : public UDPSocket
{
public:
    UDPClientSocket (char *machine,  int port);
    bool initializeClient (char * _peerAddr, int _peerPort);
    ~UDPClientSocket ( );
};

//#include "../source/UDPClientSocket.cpp"
#endif // UDPCLIENTSOCKET_H
