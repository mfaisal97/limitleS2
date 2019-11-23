#ifndef Peer_H
#define Peer_H

#include "../header/Server.h"
#include "../header/Client.h"
#include "../header/StegImage.h"

class Peer : Server{
  private:
    UserInfo userInfo;
    Client serviceDirectory;
    map<string, Client> clients;
  public:
    Peer(string _name, string _password, int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo);
    Peer(int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo);

    // local modifiers
    bool UpdateClients(map<string, ConnectionInfo> connectionsInfo);
    bool UpdateClient(string userName, ConnectionInfo connectionInfo);

    bool SetAuthInfo(string userName, string password);
    bool SetUserName(string userName);
    bool SetPassword(string password);

    bool CommunicationInfoUpdate();

    bool UpdateStegImage(string stegImageName, string stegImageContent);

    //remote modifiers
    bool RemoteSignUp();
    bool RemoteSignIn();
    bool RemoteSignOut();
    bool RemoteConnectionInfoUpdate();

    // local getters
    map<string, string> SearchForStegNames(string userName);
    Mat GetImage(string ImageID);
    bool IsClient(string userName);
    bool IsStegImage(string stegName);
    bool IsAuthorizedUpdate(string stegImageName, string stegImageContent);
    string GetUserName();

    // remote getters
    bool RemoteUpdatePeerClients();
    map<string, string> RemoteSearchForStegNames(string userName);
    bool RemoteRetrieveImage(string stegName);
    int RemoteUpdateStegImage(string stegName);

    //helpers
    int GetNextRPCID();

    ~Peer();
};

#include "../source/Peer.cpp"
#endif //Peer_H
