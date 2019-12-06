#include "../header/Peer.h"

void DebugHere(Message* firstM,  Message* m){
    // for debugging purposes
    std::cout << "\nSent: \t" + firstM->marshalString() << "\n";
    std::cout << "\nReceived: \t" + m->marshalString() << "\n";
    std::cout << "Got reply Content:\n" << (char*) m->getMessage();
    std::cout << "\n\n\n";
}

Peer::Peer(std::string _name, std::string _password, int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo) : Server("", _peerPort), serviceDirectory(serviceDirectoryHostname, _serviceDirectoryPortNo) {
  SetUserName(_name);
  SetPassword(_password);
  CommunicationInfoUpdate();
  std::cout << "Starting a new peer node on port:\t\t" << userInfo.connectionInfo.portNo << std::endl;
}

Peer::Peer(int _peerPort, char* serviceDirectoryHostname, int _serviceDirectoryPortNo) : Server("", _peerPort), serviceDirectory(serviceDirectoryHostname, _serviceDirectoryPortNo){
  SetUserName(".");
  SetPassword(".");
  CommunicationInfoUpdate();
  std::cout << "Starting a new peer node on port:\t\t" << userInfo.connectionInfo.portNo << std::endl;
}

// so important y man
Message *  Peer::doOperation(Message * message){
  std::string messageContent = FromCharArray((char*) message->getMessage());
  message->setMessageType(MessageType::Reply);

  switch (message->getOperation()) {
    case OperationType::SearchViewables:{
      std::map<std::string, std::string> res = SearchForStegNames(messageContent);
      message->setMessage((void*)ToCharArray(MapAsString(res)), MapAsString(res).length());
      break;
    }

    case OperationType::GetViewables:{
      std::string res = "";
      if(IsStegImage(messageContent)){
        res = StegImage(messageContent).AsString();
      }
      message->setMessage((void*)ToCharArray(res), res.length());
      break;
    }

    case OperationType::UpdateImage:{
      std::string res = "";
      std::string stegName = GetBetweenBrackets(&messageContent);
      res = BoolAsString(UpdateStegImage(stegName, messageContent));
      message->setMessage((void*)ToCharArray(res), res.length());
      break;
    }
    default:
      message->setMessage((void*)ToCharArray(std::string("{Unidentified Directory Service Request}")), std::string("{Unidentified Directory Service Request}").size());
  }
  return message;
}

bool Peer::UpdateClients(std::map<std::string, ConnectionInfo> connectionsInfo){
  bool done = true;
  for (auto it=connectionsInfo.begin(); it!=connectionsInfo.end(); ++it){
    std::string str = it->second.AsString();
    std::cout << "trying to connect to\t" << it->first << "\t" << str <<std::endl;
    done = done && UpdateClient(it->first, it->second);
  }
  return true;
}

bool Peer::UpdateClient(std::string userName, ConnectionInfo connectionInfo){
  clients[userName] = new Client(connectionInfo.userAddr, connectionInfo.portNo);
  return true;
}

bool Peer::SetAuthInfo(std::string userName, std::string password){
  //std::cout<<"In peercpp checking parameters"<<userName<<"  "<<password;
  return SetUserName(userName) && SetPassword(password);
}

AuthInfo Peer:: GetAuthInfo(){
  return userInfo.authInfo;
}


bool Peer::SetUserName(std::string userName){
  if (ValidUserNameString(userName)){
    userInfo.authInfo.name = userName;
    return true;
  }
  return false;
}

bool Peer::SetPassword(std::string password){
  if(ValidString(password)){
    userInfo.authInfo.password = password;
    return true;
  }
  return false;
}

// man check the format before implementation :D
bool Peer::CommunicationInfoUpdate(){
  userInfo.connectionInfo.portNo = getMyPort();
  userInfo.connectionInfo.userAddr = ToCharArray(std::string("."));
  return true;
}

std::map<std::string, std::string> Peer::SearchForStegNames(std::string userName){
  std::map<std::string, std::string> rtr;
  std::vector<std::string> filesNames = ListDirectories(ToCharArray(StegImagesDirectory));

  for(int i = 0; i < filesNames.size(); ++i){
    std::cout<<filesNames[i] <<std::endl;
    if(filesNames[i] != "." && filesNames[i] != ".."){
    StegImage image(filesNames[i]);
    std::cout<<filesNames[i]<<std::endl;
    if (image.hasViews(userName)){
        std::cout<<"has views yasta"<<std::endl;
      rtr[image.GetHash()] = image.getPlainName();
    }
  }
}

  return rtr;
}

bool Peer::UpdateStegImage(std::string stegImageName, std::string stegImageContent){
  if(IsAuthorizedUpdate(stegImageName, stegImageContent)){
    return WriteImageBinaryAsString(StegImagesDirectory, stegImageName, "png", stegImageContent);
  }
  return false;
}

StegImage Peer::GetImage(std::string ImageID){
  StegImage stegImage(ImageID);
 // cv::Mat image;

  if (stegImage.increaseViews(GetUserName(), -1)){
    stegImage.savePlainImage();
    stegImage.saveStegImage();
  }
    return stegImage;


//    image = cv::imread(PlainImagesDirectory + stegImage.getPlainName());
//    if(image.empty()) {
//      std::cout << "Image Error\n";
//      exit(-1);
//    }
//    //stegImage.removePlainImage();
//  }
//  return image;
}

bool Peer::IsClient(std::string userName){
  return clients.find(userName) != clients.end();
}

bool Peer::IsStegImage(std::string stegName){
  std::vector<std::string> files = ListDirectories(ToCharArray(StegImagesDirectory));
  return std::find(files.begin(), files.end(), stegName)!=files.end();
}

bool Peer::IsAuthorizedUpdate(std::string stegImageName, std::string stegImageContent){
  return true;
}

std::string Peer::GetUserName(){
  if(userInfo.authInfo.name!=""){
    //std::cout<<"User Info.name in get user name "<<userInfo.authInfo.name<<std::endl;
  }
  else{
    std::cout<<"User Info.name in get user name is empty"<<std::endl;
  }
  return userInfo.authInfo.name;
}

bool Peer::RemoteSignUp(){
  std::cout << "trying to signUp\n";
  std::string requeststr = userInfo.AsString();
  char* requestStrChar = ToCharArray(requeststr);
  Message* request = new Message(OperationType::SignUp, (void*)requestStrChar, requeststr.size(), GetNextRPCID());
  // DebugHere(request, request);
  Message* reply = serviceDirectory.execute(request);
  // DebugHere(request, reply);
  std::string replystr = FromCharArray((char*)reply->getMessage());
  return GetBoolBetweenBracket(&replystr);
  std::cout <<"finished signing up\n";
  return true;
}

bool Peer::RemoteSignIn(){
  Message* request = new Message(OperationType::SignIn, ToCharArray(userInfo.authInfo.AsString()), userInfo.authInfo.AsString().size(), GetNextRPCID());
  Message* reply = serviceDirectory.execute(request);
  std::string replystr = FromCharArray((char*)reply->getMessage());
  return GetBoolBetweenBracket(&replystr);
}

bool Peer::RemoteSignOut(){
  Message* request = new Message(OperationType::SignOut, ToCharArray(userInfo.authInfo.AsString()), userInfo.authInfo.AsString().size(), GetNextRPCID());
  Message* reply = serviceDirectory.execute(request);
  std::string replystr = FromCharArray((char*)reply->getMessage());
  return GetBoolBetweenBracket(&replystr);
}

bool Peer::RemoteConnectionInfoUpdate(){
  Message* request = new Message(OperationType::UpdateInfo, ToCharArray(userInfo.AsString()), userInfo.AsString().size(), GetNextRPCID());
  Message* reply = serviceDirectory.execute(request);
  std::string replystr = FromCharArray((char*)reply->getMessage());
  return GetBoolBetweenBracket(&replystr);
}

bool Peer::RemoteUpdatePeerClients(){
  clients.clear();
  Message* request = new Message(OperationType::GetOnline, ToCharArray(GetUserName()), GetUserName().size(), GetNextRPCID());
  Message* reply = serviceDirectory.execute(request);
  std::string replystr = FromCharArray((char*)reply->getMessage());
  std::map<std::string, ConnectionInfo> m = ParseConnectionInfoMap(&replystr);
  return UpdateClients(m);
}


std::map<std::string, std::string> Peer::RemoteSearchForStegNames(std::string userName){
  std::map<std::string, std::string> names;
  Message* request = new Message(OperationType::SearchViewables, ToCharArray(GetUserName()), GetUserName().size(), GetNextRPCID());
  if (RemoteUpdatePeerClients()){
    for (auto it=clients.begin(); it!=clients.end(); ++it){
      Message* reply = it->second->execute(request);
      std::string replystr = FromCharArray((char*)reply->getMessage());
      std::map<std::string, std::string> peerNames = ParseMap(&replystr);
      for (auto it2=peerNames.begin(); it2!=peerNames.end(); ++it2){
        names[it2->first] = it2->second;
      }
    }
  }
  return names;
}

bool Peer::RemoteRetrieveImage(std::string stegName){
  Message* request = new Message(OperationType::GetViewables, ToCharArray(stegName), stegName.size(), GetNextRPCID());
  if (RemoteUpdatePeerClients()){
    for (auto it=clients.begin(); it!=clients.end(); ++it){
      Message* reply = it->second->execute(request);
      std::string replystr = FromCharArray((char*)reply->getMessage());
      if(replystr.size()>0){
        bool done = WriteImageBinaryAsString(StegImagesDirectory, stegName, "png", replystr);
        if (done){
          return true;
        }
      }
    }
  }
  return false;
}

int Peer::RemoteUpdateStegImage(std::string stegName){

  int c = 0;
  if(IsStegImage(stegName)){
    std::string str = StringAsString(stegName) + StegImage(stegName).AsString();
    std::cout << "okay man calm down :D\t " << "str" << std::endl;
    Message* request = new Message(OperationType::UpdateImage, ToCharArray(str), str.size(), GetNextRPCID());
    if (RemoteUpdatePeerClients()){
      for (auto it=clients.begin(); it!=clients.end(); ++it){
        Message* reply = it->second->execute(request);
        std::string replystr = FromCharArray((char*)reply->getMessage());
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
