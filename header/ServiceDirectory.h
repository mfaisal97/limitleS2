#ifndef ServiceDirectory_H
#define ServiceDirectory_H

#include <map>
#include <vector>
#include "Server.h"
#include "../common/DataStructures.h"

class ServiceDirectory : public Server
{
    private:
      std::map<std::string, UserInfo> Users;
    public:
        ServiceDirectory(int _listen_port);

        Message * doOperation(Message * message);

        bool ValidUserName(std::string name);

        //getters
        bool IsUser(std::string name);
        //bool IsAuthorized(string name);

        bool SignUp(UserInfo userInfo);
        bool SignIn(AuthInfo authInfo);
        bool SignOut(AuthInfo authInfo);

        bool UpdateConnectionInfo(UserInfo userInfo);

        std::map<std::string, ConnectionInfo> GetOnlineUsers();

        ~ServiceDirectory();
};

//#include "../source/ServiceDirectory.cpp"
#endif // ServiceDirectory_H
