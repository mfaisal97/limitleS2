#ifndef Peer_H
#define Peer_H

#include "../header/Server.h"
#include "../header/Client.h"

class Peer : Server{
  private:
    UserInfo userInfo;
    Client serviceDirectory;
    map<string, Clinet> clients;
  public:
    Peer(string _name, string _password, int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo);

    // local modifiers
    bool UpdateClients(map<string, ConnectionInfo> connectionsInfo);
    bool UpdateClient(string userName, ConnectionInfo connectionInfo);

    bool SetAuthInfo(string userName, password);
    bool SetUserName(string userName);
    bool SetPassword(string password);

    bool CommunicationInfoUpdate();

    //remote modifiers
    bool RemoteSignUp();
    bool RemoteSignIn();
    bool RemoteSignOut();
    bool RemoteConnectionInfoUpdate();

    // local getters
    map<string, string> SearchForStegNames(string userName);
    bool UpdateStegImage(string stegImageName, string stegImageContent);
    bool ShowImage(string ImageID);
    bool IsClient(string userName);

    // remote getters
    map<string, string> RemoteSearchForStegNames(string userName);
    bool RemoteRetrieveImage(string stegName);

    ~Peer();
};

#include "../source/Peer.cpp"
#endif //Peer_H
