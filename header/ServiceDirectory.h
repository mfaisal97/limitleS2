#ifndef ServiceDirectory_H
#define ServiceDirectory_H

#include "../header/UDPServerSocket.h"
#include "../header/Message.h"
#include "../common/DataStructures.cpp"

class ServiceDirectory
{
    private:
      std::map<string, UserInfo> Users;
      UDPServerSocket udpServerSocket;
    public:
        ServiceDirectory(int _listen_port);

        //getters
        bool IsUser(string name);
        //bool IsAuthorized(string name);

        bool SignUp(UserInfo userInfo);
        bool SignIn(AuthInfo authInfo);
        bool SignOut(AuthInfo authInfo);

        bool UpdateConnectionInfo(UserInfo userInfo);

        vector<UserInfo> GetOnlineUsers();

        ~ServiceDirectory();
};

#include "../source/ServiceDirectory.cpp"
#endif // ServiceDirectory_H
