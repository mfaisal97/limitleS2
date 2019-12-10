#ifndef DataStructures_H
#define DataStructures_H

#include "utilities.h"

#include <string>
#include <map>
#include <vector>

struct AuthInfo{
  std::string name;
  std::string password;

public:
  std::string AsString(){
    std::string str = "";
    str = "\n{" + name + "}\n{" + password + "}";
    return str;
  }

  void Initialize(std::string* str){
    name = GetBetweenBrackets(str);
    password = GetBetweenBrackets(str);
  }
};

struct ConnectionInfo{
  char * userAddr;
  int portNo;

  public:
    std::string AsString(){
    std::string str = "";
    str = "\n{";
    str.append(userAddr);
    str = str + "}";
    str = str + NumberAsString(portNo);
    return str;
  }

  void Initialize(std::string* str){
    //get char*
    std::string addrstr = GetBetweenBrackets(str);
    userAddr = ToCharArray(addrstr);
    portNo = GetNumberBetweenBracket(str);
  }
};

struct UserInfo{
  AuthInfo authInfo;
  ConnectionInfo connectionInfo;
  bool online;

  std::string AsString(){
    std::string str = authInfo.AsString() + connectionInfo.AsString();
    str = str + BoolAsString(online);
    return str;
  }

  void Initialize(std::string* str){
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
  std::string plainName;
  std::string creator;
  std::map<std::string, int> remainingViews;
  std::string imageContent;

  std::string AsString(){
    return StringAsString(plainName) + StringAsString(creator) + IntMapAsString(remainingViews) + StringAsString(imageContent);
  }

  void Initialize(std::string* str){
    // std::cout << "Again size" <<  str->size()  << "\t" << std::count(str->begin(), str->end(), '}') << "\t" << str->substr(0, 100) << " ..... " << str->substr(str->size() - 2) << std::endl;
    plainName = GetBetweenBrackets(str);
    // std::cout << "parsing 01 " <<  str->size() << "\t" << std::count(str->begin(), str->end(), '}') << "\t" << str->substr(0, 100) << std::endl;
    creator = GetBetweenBrackets(str);
    // std::cout << "parsing 02 " <<  str->size() << "\t" << std::count(str->begin(), str->end(), '}') << "\t" << str->substr(0, 100) << std::endl;
    remainingViews = ParseIntMap(str);
    // std::cout << "parsing 03 " <<  str->size() << "\t" << std::count(str->begin(), str->end(), '}') << "\t" << str->substr(0, 100) << " ..... " << str->substr(str->size() - 2) << std::endl;
    imageContent = GetBetweenBrackets(str);
    // std::cout << "parsing 04 " <<  str->size() << "\t" << std::count(str->begin(), str->end(), '}') << "\t" << str->substr(0, 100)  << std::endl;
  }

  std::string GetHash(){
    // return GetStringHash(StringAsString(plainName) + StringAsString(creator) + StringAsString(imageContent));
    return GetStringHash(StringAsString(plainName) + StringAsString(creator));
  }
};

static std::string UserInfoVectorAsString(std::vector<UserInfo> v){
  std::string str = "";
  for (int i = 0; i < v.size(); ++i){
    str = str + v[i].AsString();
  }
  return str;
}

static std::string ConnectionInfoMapAsString(std::map<std::string, ConnectionInfo> users){
  std::string str = NumberAsString(users.size());
  for (auto it=users.begin(); it!=users.end(); ++it){
    str = str + StringAsString(it->first) + it->second.AsString();
  }
  return str;
}


static std::map<std::string, ConnectionInfo> ParseConnectionInfoMap(std::string* str){
  std::map<std::string, ConnectionInfo> users;
  int n = GetNumberBetweenBracket(str);
  for(int i = 0; i < n; ++i){
    std::string name = GetBetweenBrackets(str);
    ConnectionInfo ci;
    ci.Initialize(str);
    users[name] = ci;
  }
  return users;
}

#endif
