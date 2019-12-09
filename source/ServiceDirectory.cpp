#include "../header/ServiceDirectory.h"

// void DebugHere(Message* firstM,  Message* m){
//     // for debugging purposes
//     std::cout << "\nSent: \t" + firstM->marshalString() << "\n";
//     std::cout << "\nReceived: \t" + m->marshalString() << "\n";
//     std::cout << "Got reply Content:\n" << (char*) m->getMessage();
//     std::cout << "\n\n\n";
// }


ServiceDirectory::ServiceDirectory(int _listen_port) : Server(ToCharArray(""), _listen_port){
  std::cout <<"Started Directory Service:\t\ton:\t" << _listen_port << "\n";
}

Message *  ServiceDirectory::doOperation(Message * message){
  std::cout << "\nReceived: \t" + message->marshalString() << "\n";
  std::string messageContent = FromCharArray((char*) message->getMessage());
  message->setMessageType(MessageType::Reply);
  switch (message->getOperation()) {
    case OperationType::SignUp:{
      // parsing data
      UserInfo data;
      data.Initialize(&messageContent);
      // processing request
      bool res = SignUp(data);
      // writing reply
      message->setMessage((void*)ToCharArray(BoolAsString(res)), BoolAsString(res).length());
      break;
    }


    case OperationType::SignIn:{
      // parsing data
      AuthInfo data;
      data.Initialize(&messageContent);
      // processing request
      bool res = SignIn(data);
      // writing reply
      message->setMessage((void*)ToCharArray(BoolAsString(res)), BoolAsString(res).length());
      break;
    }

    case OperationType::SignOut:{
      // parsing data
      AuthInfo data;
      data.Initialize(&messageContent);
      // processing request
      bool res = SignOut(data);
      // writing reply
      message->setMessage((void*)ToCharArray(BoolAsString(res)), BoolAsString(res).length());
      break;
    }

    case OperationType::UpdateInfo:{
      // parsing data
      UserInfo data;
      data.Initialize(&messageContent);
      // processing request
      bool res = UpdateConnectionInfo(data);
      // writing reply
      message->setMessage((void*)ToCharArray(BoolAsString(res)), BoolAsString(res).length());
      break;
    }

    case OperationType::GetOnline:{
      // processing request
      std::map<std::string, ConnectionInfo> u = GetOnlineUsers();
      // writing reply
      message->setMessage((void*)ToCharArray(ConnectionInfoMapAsString(u)), ConnectionInfoMapAsString(u).length());
      break;
    }

    case OperationType::HandleReducedImage:{
      // parsing data
      StegImageInfo data;
      std::string decodedMsg=decode(messageContent);
      data.Initialize(&decodedMsg);
      // processing request
      bool res = AddReducedImage(data);
      // writing reply
      message->setMessage((void*)ToCharArray(BoolAsString(res)), BoolAsString(res).length());
      break;
    }
    default:
      message->setMessage((void*)ToCharArray(std::string("{Unidentified Directory Service Request}")), std::string("{Unidentified Directory Service Request}").size());
  }
  std::cout << "\nSent: \t" + message->marshalString() << "\n";
  return message;
}

bool ServiceDirectory::ValidUserName(std::string name){
  return (!IsUser(name)) && ValidUserNameString(name);
}

bool ServiceDirectory::SignUp(UserInfo userInfo){
  if (!ValidUserName(userInfo.authInfo.name)){
    return false;
  }

  Users.insert(std::pair<std::string ,UserInfo>(userInfo.authInfo.name, userInfo));
  UpdateConnectionInfo(userInfo);
  return true;
}


bool ServiceDirectory::IsUser(std::string name){
  std::map<std::string,UserInfo>::const_iterator it = Users.find(name);
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

      char str[INET_ADDRSTRLEN];
      struct sockaddr_in sa = udpServerSocket->getPeerAddr();
      inet_ntop(AF_INET,&(sa.sin_addr), str, INET_ADDRSTRLEN);

      std::cout << str << std::endl;

      it->second.connectionInfo.userAddr = ToCharArray(str);
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

bool ServiceDirectory::AddReducedImage(const StegImageInfo& stegImageInfo){
  StegImage stegImage(stegImageInfo);
  return stegImage.saveStegImage();
}



std::map<std::string, ConnectionInfo> ServiceDirectory::GetOnlineUsers(){
  std::map<std::string, ConnectionInfo> onlineUsers;

  for (std::map<std::string, UserInfo>::iterator it=Users.begin(); it!=Users.end(); ++it){
    if(it->second.online){
      onlineUsers[it->first] = it->second.connectionInfo;
    }
  }

  return onlineUsers;
}


ServiceDirectory::~ServiceDirectory(){

}
