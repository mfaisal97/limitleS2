#ifndef Peer_H
#define Peer_H

#include "../header/Server.h"
#include "../header/Client.h"

class Peer : Server
{
    private:
      UserInfo userInfo;
      map<string, Clinet> clients;
    public:
        Peer(string name, string password, int port);

        bool SignIn();
        bool SignOut();
        bool SignUp();
        bool UpdateConnectionInfo();

        bool UpdateClients();

        vector<string> SearchForRemoteViewables();
        vector<string> SearchForLocalViewables();
        bool ShowImage(string ImageID)

        ~Peer();
};

#include "../source/Peer.cpp"
#endif //Peer_H
