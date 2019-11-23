#include <string>

#include "../stegn/decode.cpp"
#include "../stegn/encode.cpp"

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
    str.append(userAddr);
    str = str + "}";
    str = str + NumberAsString(portNo);
    return str;
  }

  void Initialize(string* str){
    //get char*
    string addrstr = GetBetweenBrackets(str);
    userAddr = ToCharArray(addrstr);
    portNo = GetNumberBetweenBracket(str);
  }
};

struct UserInfo{
  AuthInfo authInfo;
  ConnectionInfo connectionInfo;
  bool online;

  string AsString(){
    string str = authInfo.AsString() + connectionInfo.AsString();
    str = str + BoolAsString(online);
    return str;
  }

  void Initialize(string* str){
    authInfo.Initialize(str);
    connectionInfo.Initialize(str);
    online = GetBoolBetweenBracket(str);
  }
};

// Image needs to be same level as its data
// if data written inside picture,
// you lose the default picture
// after multiple writes
struct StegImageInfo{
  string plainName;
  string creator;
  map<string, int> remainingViews;
  string imageContent;

  string AsString(){
    return StringAsString(plainName) + StringAsString(creator) + IntMapAsString(remainingViews) + StringAsString(imageContent);
  }

  void Initialize(string* str){
    plainName = GetBetweenBrackets(&str);
    creator = GetBetweenBrackets(&str);
    remainingViews = ParseIntMap(&str);
    imageContent = GetBetweenBrackets(&str);
  }

  string GetHash(){
    return GetStringHash(StringAsString(plainName) + StringAsString(creator) + StringAsString(imageContent));
  }
};

string UserInfoVectorAsString(vector<UserInfo> v){
  string str = "";
  for (int i = 0; i < v.size(); ++i){
    str = str + v[0].AsString();
  }
  return str;
}
