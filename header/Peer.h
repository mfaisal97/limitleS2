#ifndef Peer_H
#define Peer_H

#include "../header/StegImage.h"
#include "../header/Server.h"
#include "../header/Client.h"

#include "../common/utilities.h"

#include <opencv2/core/mat.hpp>
#include <string>
#include <map>

class Peer : Server{
  private:
    UserInfo userInfo;
    Client serviceDirectory;
    std::map<std::string, Client*> clients;
    bool* serverRunning;
  public:
    Peer(std::string _name, std::string _password, int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo);
    Peer(int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo);

    Message * doOperation(Message * message);

    // local modifiers
    bool UpdateClients(std::map<std::string, ConnectionInfo> connectionsInfo);
    bool UpdateClient(std::string userName, ConnectionInfo connectionInfo);

    bool SetAuthInfo(std::string userName, std::string password);
    AuthInfo GetAuthInfo();
    bool SetUserName(std::string userName);
    bool SetPassword(std::string password);

    bool CommunicationInfoUpdate();

    bool UpdateStegImage(std::string stegImageName, std::string stegImageContent);

    //remote modifiers
    bool RemoteSignUp();
    bool RemoteSignIn();
    bool RemoteSignOut();
    bool RemoteConnectionInfoUpdate();

    // local getters
    std::map<std::string, std::string> SearchForStegNames(std::string userName);
    cv::Mat GetImage(std::string ImageID);
    bool IsClient(std::string userName);
    bool IsStegImage(std::string stegName);
    bool IsAuthorizedUpdate(std::string stegImageName, std::string stegImageContent);
    std::string GetUserName();

    // remote getters
    bool RemoteUpdatePeerClients();
    std::map<std::string, std::string> RemoteSearchForStegNames(std::string userName);
    bool RemoteRetrieveImage(std::string stegName);
    int RemoteUpdateStegImage(std::string stegName);

    //helpers
    int GetNextRPCID();
    void startPeerServer();

    ~Peer();
};

//#include "../source/Peer.cpp"
#endif //Peer_H
