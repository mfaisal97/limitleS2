#include "../header/StegImage.h"

StegImage::StegImage(std::string plainName, std::string creator){
    //std::cout<<"Entered Constructor\t"<<"Image Name: "<<plainName<<std::endl;
    addImage(plainName);
    setCreator(creator);
    //std::cout<<"Steg Image Created"<<std::endl;
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
  std::cout << "Loading Steg Name " << StegImagesDirectory + "/" + stegName << std::endl;
  std::vector<std::string> files = ListFiles(ToCharArray(StegImagesDirectory + "/" + stegName ));
  std::string InfoString = decodeAllInDirectory(StegImagesDirectory + "/" + stegName, files);
  // std::cout << InfoString << std::endl;
  info.Initialize(&InfoString);
}

bool StegImage::addImage(std::string plainName){
  info.plainName = plainName;
  int extInd = plainName.find('.');
  info.imageContent = ReadImageBinaryAsString(PlainImagesDirectory, plainName.substr(0, extInd), plainName.substr(extInd + 1));
  std::cout << "Starting size\t" << info.imageContent.length() << "\t" << std::count(info.imageContent.begin(), info.imageContent.end(), '}') <<std::endl;
  return true;
}

bool StegImage::addUser(std::string userName, int views){
  if(ValidUserNameString(userName)){
    info.remainingViews[userName] = views;
    return true;
  }

  return false;
}

StegImageInfo StegImage::getStegImageInfo(){
  return info;
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

bool StegImage::changePlainName(std::string plainName){
  info.plainName = plainName;
  return true;
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

std::string StegImage::getStegName(){
  return info.GetHash();
}

std::string StegImage::AsString(){
  std::string str = info.AsString();
  std::string hash = info.GetHash();
  int numberOfFiles = getNumberOfFiles(str.size());

  bool created = createDirectoryWithFiles(StegImagesDirectory + "/" + hash, numberOfFiles, str);
  std::string encoding = str;

  return encoding;
}

char* StegImage::AsCharArray(){
  return ToCharArray(AsString());
}

bool StegImage::savePlainImage(){
  int extInd =  info.plainName.find('.');
  std::cout << "Ending size\t" << info.imageContent.length()<< "\t" << std::count(info.imageContent.begin(), info.imageContent.end(), '}') <<std::endl;
  return WriteImageBinaryAsString(PlainImagesDirectory,  info.plainName.substr(0, extInd),  info.plainName.substr(extInd + 1), info.imageContent);
}

bool StegImage::removePlainImage(){
  return remove(ToCharArray(PlainImagesDirectory + "/" + info.plainName)) == 0;
}

bool StegImage::saveStegImage(){
  std::string str = info.AsString();
  std::string hash = info.GetHash();
  int numberOfFiles = getNumberOfFiles(str.size());

  bool created = createDirectoryWithFiles(StegImagesDirectory + "/" + hash, numberOfFiles, str);

  //std::string encoding = "";
  // std::cout << "got the following encoding \t" << encoding << std::endl;
  return true;
}

StegImage::~StegImage(){
}
