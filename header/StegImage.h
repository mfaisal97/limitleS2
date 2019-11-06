#ifndef StegImage_H
#define StegImage_H

#include "../header/Server.h"
#include "../header/Client.h"
#include "../header/Message.h"

class StegImage
{
    private:

    public:
        StegImage();
        StegImage(string stegImagePath);      //open already StegImage
        ~StegImage();
        void AddImage(string fileName);
        void AddUser(string userName, int views);
        void Creator(string userName);


        //add some modifiers


        //add some getters


        //

        char* CreateStegImage();    //returns the default image with everything inside
};

#include "../source/StegImage.cpp"
#endif //StegImage_H
