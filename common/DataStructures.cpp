#include <string>

using namespace std;

struct AuthInfo{
  string name;
  string password;
};

struct ConnectionInfo{
  char * userAddr;
  int portNo;
}

struct UserInfo{
  AuthInfo authInfo;
  ConnectionInfo connectionInfo;
  bool online;
};
