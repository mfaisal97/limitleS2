#include "../header/StegImage.h"

StegImage::StegImage(std::string plainName, std::string creator){
    addImage(plainName);
    setCreator(creator);
}

StegImage::StegImage(std::string plainName, std::string creator, std::map<std::string, int> remainingViews):StegImage(plainName, creator){
  info.remainingViews = remainingViews;
}

StegImage::StegImage(StegImageInfo _info){
  info.plainName = _info.plainName;
  info.creator = _info.creator;
  info.remainingViews = _info.remainingViews;
  info.imageContent = _info.imageContent;
}

StegImage::StegImage(std::string stegName){
  std::string InfoString = Decode("", StegImagesDirectory + stegName, true);
  info.Initialize(&InfoString);
}

bool StegImage::addImage(std::string plainName){
  info.plainName = plainName;
  info.imageContent = ReadImageBinaryAsString(PlainImagesDirectory + plainName);

  return true;
}

bool StegImage::addUser(std::string userName, int views){
  if(ValidUserNameString(userName)){
    info.remainingViews[userName] = views;
    return true;
  }

  return false;
}

bool StegImage::increaseViews(std::string userName, int inc){
  if(canIncreaseViews(userName, inc)){
    info.remainingViews[userName] = info.remainingViews[userName] + inc;
    return true;
  }

  return false;
}

bool StegImage::setCreator(std::string userName){
  if(ValidUserNameString(userName)){
    info.creator = userName;
    return true;
  }
  return false;
}


bool StegImage::canIncreaseViews(std::string name, int inc){
  return (getUserViews(name) >= 0) &&  (getUserViews(name) + inc >= 0);
}

bool StegImage::hasViews(std::string name){
  return getUserViews(name) > 0;
}

int StegImage::getUserViews(std::string name){
  auto it = info.remainingViews.find(name);
  if (it != info.remainingViews.end()){
    if (info.remainingViews[name] >= 0){
      return info.remainingViews[name];
    }
  }
  return -1;
}

std::string StegImage::getCreator(){
  return info.creator;
}

std::string StegImage::getPlainName(){
  return info.plainName;
}

std::string StegImage::AsString(){
  std::string hash = info.GetHash();
  std::string encoding = Encode(info.AsString(), StegImagesDirectory + hash + ".jpeg");
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
  std::string hash = info.GetHash();
  std::string encoding = Encode(info.AsString(), StegImagesDirectory + hash + ".jpeg");
  return true;
}

StegImage::~StegImage(){
}
