#ifndef StegImage_H
#define StegImage_H

#include "../common/DataStructures.cpp"

class StegImage{
    private:
        StegImageInfo info;
    public:
        StegImage(string plainName, string creator);
        StegImage(string plainName, string creator, map<string, int> remainingViews);
        StegImage(StegImageInfo _info);
        StegImage(string stegName);      //open already StegImage

        //add some modifiers
        bool addImage(string plainName);
        bool addUser(string userName, int views);
        bool increaseViews(string userName, int inc);
        bool setCreator(string userName);

        //getters
        bool canIncreaseViews(string name, int inc);
        bool hasViews(string name);
        int getUserViews(string name);
        string getCreator();
        string getPlainName();

        string AsString();
        char* AsCharArray();

        // helpers
        bool savePlainImage();
        bool removePlainImage();
        bool saveStegImage();
        string GetHash(){
          return info.GetHash();
        }

        ~StegImage();
};

#include "../source/StegImage.cpp"
#endif //StegImage_H
