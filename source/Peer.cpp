Peer::Peer(string _name, string _password, int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo) : Server("", _peerPort), serviceDirectory(serviceDirectoryHostname, _serviceDirectoryPortNo) {
  SetUserName(_name);
  SetPassword(_password);
  CommunicationInfoUpdate();
  cout << "Starting a new peer node on port:\t\t" << userInfo.connectionInfo.port << endl;
}

Peer::Peer(int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo) : Server("", _peerPort), serviceDirectory(serviceDirectoryHostname, _serviceDirectoryPortNo){
  CommunicationInfoUpdate();
  cout << "Starting a new peer node on port:\t\t" << userInfo.connectionInfo.port << endl;
}

// so important y man
Message *  Peer::doOperation(Message * message){
  string messageContent = FromCharArray((char*) message->getMessage());
  message->setMessageType(MessageType::Reply);

  switch (message->getOperation()) {
    case OperationType::SearchViewables:{
      map<string, string> res = SearchForStegNames(messageContent);
      message->setMessage((void*)ToCharArray(MapAsString(res)), MapAsString(res).length());
      break;
    }

    case OperationType::GetViewables:{
      string res = "";
      if(IsStegImage(messageContent)){
        res = StegImage(messageContent).AsString();
      }
      message->setMessage((void*)ToCharArray(res), res.length());
      break;
    }

    case OperationType::UpdateImage:{
      string res = "";
      string stegName = GetBetweenBrackets(&messageContent);
      res = BoolAsString(UpdateStegImage(stegName, messageContent);
      message->setMessage((void*)ToCharArray(res), res.length());
      break;
    }
    default:
      message->setMessage((void*)ToCharArray(string("{Unidentified Directory Service Request}")), string("{Unidentified Directory Service Request}").size());
  }
  return message;
}

bool Peer::UpdateClients(map<string, ConnectionInfo> connectionsInfo){
  bool done = true;
  for (auto it=connectionsInfo.begin(); it!=connectionsInfo.end(); ++it){
    done = done && UpdateClient(it->first, it->second);
  }
  return true;
}

bool Peer::UpdateClient(string userName, ConnectionInfo connectionInfo){
  clients[userName] = Client(connectionInfo.userAddr, connectionInfo.portNo);
  return true;
}

bool Peer::SetAuthInfo(string userName, password){
  return SetUserName(userName) && SetPassword(password);
}

bool Peer::SetUserName(string userName){
  if (ValidUserNameString(userName)){
    userInfo.authInfo.name = userName;
    return true;
  }
  return false;
}

bool Peer::SetPassword(string password){
  if(ValidString(password)){
    userInfo.password = password;
    return true;
  }
  return false;
}

// man check the format before implementation :D
bool Peer::CommunicationInfoUpdate(){
  userInfo.connectionInfo.portNo = getMyPort();
  return true;
}

map<string, string> Peer::SearchForStegNames(string userName){
  map<string, string> rtr;
  vector<string> filesNames = ListFiles(ToCharArray(StegImagesDirectory));

  for(int i = ; i < filesNames.size(); ++i){
    StegImage image(filesNames[i] + ".jpeg");
    if (image.hasViews(userName)){
      rtr[image.GetHash()] = image.getPlainName();
    }
  }

  return rtr;
}

bool Peer::UpdateStegImage(string stegImageName, string stegImageContent){
  if(IsAuthorizedUpdate(stegImageName, stegImageContent)){
    return WriteImageBinaryAsString(StegImagesDirectory + hash + ".jpeg", stegImageContent);
  }
  return false;
}

Mat Peer::GetImage(string ImageID){
  StegImage stegImage(ImageID + ".jpeg");
  Mat image;

  if (stegImage.increaseViews(GetUserName(), -1)){
    stegImage.savePlainImage();
    stegImage.saveStegImage();

    image = imread(PlainImagesDirectory + stegImage.getPlainName());
    if(image.empty()) {
      cout << "Image Error\n";
      exit(-1);
    }
    stegImage.removePlainImage();
  }
  return image;
}

bool Peer::IsClient(string userName){
  return clients.find(userName) != clients.end();
}

bool Peer::IsAuthorizedUpdate(string stegImageName, string stegImageContent){
  return true;
}

string Peer::GetUserName(){
  return userInfo.authInfo.name;
}

bool Peer::RemoteSignUp(){
  Message request(OperationType::SignUp, ToCharArray(userInfo.AsString()), userInfo.AsString().size(), GetNextRPCID());
  Message reply = serviceDirectory.execute(request);
  return GetBoolBetweenBracket(FromCharArray((char*)reply.getMessage()));
}

bool Peer::RemoteSignIn(){
  Message request(OperationType::SignIn, ToCharArray(userInfo.authInfo.AsString()), userInfo.authInfo.AsString().size(), GetNextRPCID());
  Message reply = serviceDirectory.execute(request);
  return GetBoolBetweenBracket(FromCharArray((char*)reply.getMessage()));
}

bool Peer::RemoteSignOut(){
  Message request(OperationType::SignOut, ToCharArray(userInfo.authInfo.AsString()), userInfo.authInfo.AsString().size(), GetNextRPCID());
  Message reply = serviceDirectory.execute(request);
  return GetBoolBetweenBracket(FromCharArray((char*)reply.getMessage()));
}

bool Peer::RemoteConnectionInfoUpdate(){
  Message request(OperationType::UpdateInfo, ToCharArray(userInfo.AsString()), userInfo.AsString().size(), GetNextRPCID());
  Message reply = serviceDirectory.execute(request);
  return GetBoolBetweenBracket(FromCharArray((char*)reply.getMessage()));
}

bool Peer::RemoteUpdatePeerClients(){
  clients.clear();
  Message request(OperationType::GetOnline, ToCharArray(GetUserName()), GetUserName().size(), GetNextRPCID());
  Message reply = serviceDirectory.execute(request);
  map<string, ConnectionInfo> m = ParseConnectionInfoMap(FromCharArray((char*)reply.getMessage()));
  return UpdateClients(m);
}


map<string, string> Peer::RemoteSearchForStegNames(string userName){
  map<string, string> names;
  Message request(OperationType::SearchViewables, ToCharArray(GetUserName()), GetUserName().size(), GetNextRPCID());
  if (RemoteUpdatePeerClients()){
    for (auto it=clients.begin(); it!=clients.end(); ++it){
      Message reply = it->second.execute(request);
      map<string, string> peerNames = ParseMap(FromCharArray((char*)reply.getMessage()));
      for (auto it2=peerNames.begin(); it2!=peerNames.end(); ++it2){
        names[it2->first] = it2->second;
      }
    }
  }
  return names;
}

bool Peer::RemoteRetrieveImage(string stegName){
  Message request(OperationType::GetViewables, ToCharArray(stegName), stegName.size(), GetNextRPCID());
  if (RemoteUpdatePeerClients()){
    for (auto it=clients.begin(); it!=clients.end(); ++it){
      Message reply = it->second.execute(request);
      string replystr = FromCharArray((char*)reply.getMessage());
      if(replystr.size()>0){
        bool done = WriteImageBinaryAsString(StegImagesDirectory + stegName + ".jpeg", replystr);
        if (done){
          return true;
        }
      }
    }
  }
  return false;
}

int Peer::RemoteUpdateStegImage(string stegName){
  int c = 0;
  if(IsStegImage(stegName)){
    string str = StringAsString(stegName) + StegImage(stegName).AsString();
    Message request(OperationType::UpdateImage, ToCharArray(str), str.size(), GetNextRPCID());
    if (RemoteUpdatePeerClients()){
      for (auto it=clients.begin(); it!=clients.end(); ++it){
        Message reply = *it->second.execute(&request);
        string replystr = FromCharArray((char*)reply.getMessage());
        if (GetBoolBetweenBracket(&replystr)){
          ++c;
        }
      }
    }
  }
  return c;
}

int Peer::GetNextRPCID(){
  return 0;
}

Peer::~Peer(){
}
