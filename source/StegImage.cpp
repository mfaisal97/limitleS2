StegImage::StegImage(string plainName, string creator){
    addImage(plainName);
    setCreator(creator);
}

StegImage::StegImage(string plainName, string creator, map<string, int> remainingViews):StegImage(plainName, creator){
  info.remainingViews = remainingViews;
}

StegImage::StegImage(StegImageInfo _info){
  info.plainName = _info.plainName;
  info.creator = _info.creator;
  info.remainingViews = _info.remainingViews;
  info.imageContent = _info.imageContent;
}

StegImage::StegImage(string stegName){
  string InfoString = decode("", StegImagesDirectory + stegName, true);
  info.Initialize(&InfoString);
}

bool StegImage::addImage(string plainName){
  info.plainName = plainName;
  info.imageContent = ReadImageBinaryAsString(PlainImagesDirectory + plainName);

  return true;
}

bool StegImage::addUser(string userName, int views){
  if(ValidUserNameString(userName)){
    info.remainingViews[userName] = views;
    return true;
  }

  return false;
}

bool StegImage::increaseViews(string userName, int inc){
  if(canIncreaseViews(userName, inc)){
    info.remainingViews[userName] = info.remainingViews[userName] + inc;
    return true;
  }

  return false;
}

bool StegImage::setCreator(string userName){
  if(ValidUserNameString(userName)){
    info.creator = userName;
    return true;
  }
  return false;
}


bool StegImage::canIncreaseViews(string name, int inc){
  return (getUserViews(name) >= 0) &&  (getUserViews(name) + inc >= 0);
}

bool StegImage::hasViews(string name){
  return getUserViews(name) > 0;
}

int StegImage::getUserViews(string name){
  auto it = info.remainingViews.find(name);
  if (it != info.remainingViews.end()){
    if (info.remainingViews[name] >= 0){
      return info.remainingViews[name];
    }
  }
  return -1;
}

string StegImage::getCreator(){
  return info.creator;
}

string StegImage::getPlainName(){
  return info.plainName;
}

string StegImage::AsString(){
  string hash = info.GetHash();
  string encoding = Encode(info.AsString(), StegImagesDirectory + hash + ".jpeg");
  return encoding;
}

char* StegImage::AsCharArray(){
  return ToCharArray(AsString());
}

bool StegImage::savePlainImage(){
  return WriteImageBinaryAsString(PlainImagesDirectory + info.plainName, info.imageContent);
}

bool StegImage::removePlainImage(){
  return remove(ToCharArray(PlainImagesDirectory + info.plainName)) == 0;
}

bool StegImage::saveStegImage(){
  string hash = info.GetHash();
  string encoding = Encode(info.AsString(), StegImagesDirectory + hash + ".jpeg");
  return true;
}

StegImage::~StegImage(){
}
