#ifndef Peer_H
#define Peer_H

#include "../header/Server.h"
#include "../header/Client.h"
#include "../header/Message.h"

class Peer
{
    private:
      Server server;
      Client client;
    public:
        Peer(int _listen_port);
        ~Peer();
};

#include "../source/Peer.cpp"
#endif //Peer_H
