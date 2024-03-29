#ifndef Peer_H
#define Peer_H

#include "../header/StegImage.h"
#include "../header/Server.h"
#include "../header/Client.h"
#include "vector"

#include "../common/utilities.h"

#include <opencv2/core/mat.hpp>
#include <string>
#include <map>

class Peer : public Server{
  private:
    UserInfo userInfo;
    Client serviceDirectory;
    std::vector<std::string>notifications;
    std::map<std::string, ConnectionInfo> clients;
  public:
    Peer(std::string _name, std::string _password, int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo);
    Peer(int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo);

    Message * doOperation(Message * message);

    // local modifiers
    bool UpdateClients(std::map<std::string, ConnectionInfo> connectionsInfo);
    bool UpdateClient(std::string userName, ConnectionInfo connectionInfo);
    bool SendNotification(std::string msg, std::string client_user);

    bool SetAuthInfo(std::string userName, std::string password);
    AuthInfo GetAuthInfo();
    bool SetUserName(std::string userName);
    bool SetPassword(std::string password);
    std::vector<std::string> getOnlineUsers();
    bool CommunicationInfoUpdate();

    bool UpdateStegImage(std::string stegImageName, std::string stegImageContent);

    //remote modifiers
    bool RemoteSignUp();
    bool RemoteSignIn();
    bool RemoteSignOut();
    bool RemoteConnectionInfoUpdate();

    // local getters
    std::map<std::string, std::string> SearchForStegNames(std::string userName);
    StegImage GetImage(std::string ImageID);
    bool IsClient(std::string userName);
    bool IsStegImage(std::string stegName);
    bool IsAuthorizedUpdate(std::string stegImageName, std::string stegImageContent);
    std::string GetUserName();

    // remote getters
    bool RemoteUpdatePeerClients();
    std::map<std::string, std::string> RemoteSearchForStegNames(std::string userName);
    std::vector<std::string> GetNotifications(){
      return notifications;
    }
    bool RemoteRetrieveImage(std::string stegName);
    bool RemoteRetrievePartNumber(std::string hash,int partNumber);
    bool RemoteRetrieveAllParts(std::string hash,int numberOfParts);
    bool RemoteUpdateReducedStegImage(std::string stegName);
    int RemoteUpdateStegImage(std::string stegName);

    //helpers
    int GetNextRPCID();
    // void startPeerServer();

    ~Peer();
};

//#include "../source/Peer.cpp"
#endif //Peer_H
