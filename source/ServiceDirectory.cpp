
ServiceDirectory::ServiceDirectory(int _listen_port) : Server("", _listen_port){
  cout <<"Started Directory Service:\t\ton:\t" << _listen_port << "\n";
}

Message *  ServiceDirectory::doOperation(Message * message){
  return new Message("y wad");
}

bool ServiceDirectory::SignUp(UserInfo userInfo){
  if (IsUser(userInfo.authInfo.name)){
    return false;
  }

  Users.insert(std::pair<string ,UserInfo>(userInfo.authInfo.name, userInfo));
  return true;
}


bool ServiceDirectory::IsUser(string name){
  map<string,UserInfo>::const_iterator it = Users.find(name);
  return it!=Users.end();
}

bool ServiceDirectory::SignIn(AuthInfo authInfo){
  auto it = Users.find(authInfo.name);
  if (it!= Users.end()){
    //User exist
    if(authInfo.password == it->second.authInfo.password){
      it->second.online = true;
      return true;
    }
  }
  return false;
}


bool ServiceDirectory::UpdateConnectionInfo(UserInfo userInfo){
  auto it = Users.find(userInfo.authInfo.name);
  if (it!= Users.end()){
    //User exist
    if(userInfo.authInfo.password == it->second.authInfo.password){
      it->second.connectionInfo = userInfo.connectionInfo;
      return true;
    }
  }
  return false;
}


bool ServiceDirectory::SignOut(AuthInfo authInfo){
  auto it = Users.find(authInfo.name);
  if (it!= Users.end()){
    //User exist
    if(authInfo.password == it->second.authInfo.password){
      it->second.online = false;
      return true;
    }
  }
  return false;
}


vector<UserInfo> ServiceDirectory::GetOnlineUsers(){
  std::vector<UserInfo> onlineUsers;

  for (std::map<string, UserInfo>::iterator it=Users.begin(); it!=Users.end(); ++it){
    if(it->second.online){
      onlineUsers.push_back(it->second);
    }
  }

  return onlineUsers;
}


ServiceDirectory::~ServiceDirectory(){

}
