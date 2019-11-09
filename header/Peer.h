#ifndef Peer_H
#define Peer_H

#include "../header/Server.h"
#include "../header/Client.h"
#include "../header/Message.h"

class Peer
{
    private:
      UserInfo userInfo;
      Server server;
    public:
        Peer(string name, string password, int port);
        SignIn();
        ~Peer();
};

#include "../source/Peer.cpp"
#endif //Peer_H
