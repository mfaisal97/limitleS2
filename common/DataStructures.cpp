#include <string>

#include "utilities.cpp"

using namespace std;

struct AuthInfo{
  string name;
  string password;

public:
  string AsString(){
    string str = "";
    str = "\n{" + name + "}\n{" + password + "}";
    return str;
  }

  void Initialize(string* str){
    name = GetBetweenBrackets(str);
    password = GetBetweenBrackets(str);
  }
};

struct ConnectionInfo{
  char * userAddr;
  int portNo;

  public:
    string AsString(){
    string str = "";
    str = "\n{";
    str.append(name);
    str = str + "}\n{" + portNo + "}";
    return str;
  }

  void Initialize(string* str){
    //get char*
    string addrstr = GetBetweenBrackets(str);
    userAddr = new char[addrstr.size() + 1]
    strcpy(userAddr, addrstr.c_str());

    password = GetNumberBetweenBracket(str);
  }
};

struct UserInfo{
  AuthInfo authInfo;
  ConnectionInfo connectionInfo;
  bool online;

  string AsString(){
    string str = authInfo.AsString() + connectionInfo.AsString();
    str = str + "\n{" + (online? "1":"0") + "}";
    return str;
  }

  void Initialize(string* str){
    authInfo.Initialize(str);
    connectionInfo.Initialize(str);
    online = GetNumberBetweenBracket(str);
  }
};
