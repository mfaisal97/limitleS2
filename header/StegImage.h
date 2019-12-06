#ifndef StegImage_H
#define StegImage_H

#include "../common/common.h"
#include <string>
#include <vector>
#include <map>

class StegImage{
    private:
        StegImageInfo info;
    public:
        StegImage(std::string plainName, std::string creator);
        StegImage(std::string plainName, std::string creator, std::map<std::string, int> remainingViews);
        StegImage(StegImageInfo _info);
        StegImage(std::string stegName);      //open already StegImage

        //add some modifiers
        bool addImage(std::string plainName);
        bool addUser(std::string userName, int views);
        bool increaseViews(std::string userName, int inc);
        bool setCreator(std::string userName);
        bool changePlainName(std::string plainName);

        //getters
        bool canIncreaseViews(std::string name, int inc);
        bool hasViews(std::string name);
        int getUserViews(std::string name);
        std::string getCreator();
        std::string getPlainName();
        std::string getImageContent(){
            return info.imageContent;
        }

        StegImageInfo getStegImageInfo();

        std::string AsString();
        char* AsCharArray();

        // helpers
        bool savePlainImage();
        bool removePlainImage();
        bool saveStegImage();
        std::string GetHash(){
          return info.GetHash();
        }

        ~StegImage();
};

//#include "../source/StegImage.cpp"
#endif //StegImage_H
