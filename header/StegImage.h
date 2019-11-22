#ifndef StegImage_H
#define StegImage_H


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
