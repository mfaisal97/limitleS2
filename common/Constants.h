#ifndef Constants_H
#define Constants_H

#include <string>

const std::string StegPassword = "Ay 7aga";
const std::string DefaultImagePath = "Data/def/defImage.jpeg";
const std::string PlainImagesDirectory = "Data/images/plain/";
const std::string StegImagesDirectory = "Data/images/Steg/";
const int MAX_MESSAGE_SIZE = 1024;

//config
static int PeerServerPort = 4051;
static int ServiceDirectoryPort = 4050;
static std::string ServiceDirectoryAddr = "10.40.52.90";

#endif
