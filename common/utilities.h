#ifndef UTILITIES_H
#define UTILITIES_H

#include "Constants.h"

// #include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
// #include <algorithm>
// #include <cctype>
// #include <cerrno>
// #include <cstdio>
// #include <stdlib.h>
// #include <functional>
// #include <sys/stat.h>
// #include <string.h>

#include <fstream>
#include <iostream>
#include <dirent.h>


#include <string>
#include <vector>
#include <map>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


static void setBroadcast(int s){
    int arg;
    #ifdef SO_BROADCAST
    arg =1;
    if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &arg, sizeof(arg)) <0){
        perror("setsockopt SO_BROADCAST---");
        exit(-1);
    }
    #endif
}

static void makeLocalSA(struct sockaddr_in *sa){
    sa->sin_family = AF_INET;
    sa->sin_port = htons(0);
    sa-> sin_addr.s_addr = htonl(INADDR_ANY);
}
static void makeDestSA(struct sockaddr_in * sa, char *hostname, int port){ struct hostent *host;
    sa->sin_family = AF_INET;
    if((host = gethostbyname(hostname))== NULL){
        printf("Unknown host name\n");
        exit(-1);
    }
    sa-> sin_addr = *(struct in_addr *) (host->h_addr);
    sa->sin_port = htons(port);
}

static void makeReceiverSA(struct sockaddr_in *sa, int port){
    sa->sin_family = AF_INET;
    sa->sin_port = htons(port);
    sa-> sin_addr.s_addr = htonl(INADDR_ANY);
}

static std::string GetBetweenBrackets(std::string* str){
  int start = str->find("{");
  int end = str->find("}");
  std::string rt = "";

  if (end >= 0 && start >= 0 && end - start >= 2){
      rt = str->substr(start + 1, end - start -1);
      *str = str->substr(end+1);
  }

  return rt;
}

static int GetNumberBetweenBracket(std::string* str){
  std::string numStr = GetBetweenBrackets(str);
  return stoi(numStr);
}

static bool GetBoolBetweenBracket(std::string* str){
  std::string numStr = GetBetweenBrackets(str);
  if(numStr=="1"){
    std::cout<<"\n Sign in:\t"<<"Succeeded"<<'\n';}
  else{
    std::cout<<"\n Sign up:\t"<<"Failed"<<'\n';
  }
  return stoi(numStr);
}
static std::string StringAsString(std::string str){
  return "\n{" + str + "}";
}

static std::string BoolAsString(bool x){
  return x?"\n{1}" : "\n{0}";
}

static std::string NumberAsString(int n){
  return "\n{" + std::to_string(n) + "}";
}

static std::string MapAsString(std::map<std::string, std::string> m){
  std::string str = "";
  for (std::map<std::string, std::string>::iterator it=m.begin(); it!=m.end(); ++it){
    str = str + "\n{" + it->first + "}\n{" + it->second + "}";
  }
  return str;
}

static std::string IntMapAsString(std::map<std::string, int> m){
  std::string str = "";
  for (std::map<std::string, int>::iterator it=m.begin(); it!=m.end(); ++it){
    str = str + "\n{" + it->first + "}\n{" + std::to_string(it->second) + "}";
  }
  return str;
}

static std::map<std::string, std::string> ParseMap(std::string * str){
  std::map<std::string, std::string> m;
  while(str->length()>0){
    std::string key = GetBetweenBrackets(str);
    std::string val = GetBetweenBrackets(str);
    m[key] = val;
  }
  return m;
}

static std::map<std::string, int> ParseIntMap(std::string * str){
  std::map<std::string, int> m;
  while(str->length()>0){
    std::string key = GetBetweenBrackets(str);
    int val = GetNumberBetweenBracket(str);
    m[key] = val;
  }
  return m;
}

static char* ToCharArray(std::string str){
  char* strcharacters = new char[str.size() + 1];
  str.copy(strcharacters, str.size() + 1);
  strcharacters[str.size()] = '\0';

  return strcharacters;
}

static std::string FromCharArray(char* array){
  std::string str = "";
  int i = 0;

  while (array[i] != '\0'){
    char c[1];
    c[0] = array[i++];
    str.append(&c[0]);
  }

  return str;
}

static bool ValidString(std::string str){
  return !(str.find('{') != std::string::npos || str.find('}') != std::string::npos || str.size() == 0);
}

static bool ValidUserNameString(std::string name){
  return (!any_of(name.begin(), name.end(), ::isdigit)) && ValidString(name);
}

static bool WriteImageBinaryAsString(std::string imageFullPath, std::string content){
  std::ofstream wf(imageFullPath, std::ios::out | std::ios::binary);
	if(!wf) {
		std::cout << "Cannot open Fake \"" + imageFullPath + "\" for writing" << std::endl;
		return false;
	}
	wf.write((char *) &content, sizeof(content));
	wf.close();
  return true;
}

static std::string ReadImageBinaryAsString(std::string imageFullPath){
  std::string content = "";
  std::ifstream rf(imageFullPath, std::ios::binary);
  if (rf){
    // get length of file:
    rf.seekg (0, rf.end);
    int length = rf.tellg();
    rf.seekg (0, rf.beg);

    char * buffer = new char [length];
    rf.read (buffer,length);
    content = FromCharArray(buffer);

    delete[] buffer;
    rf.close();
  }else {
    std::cout << "Cannot open \"" + imageFullPath + "\" for reading" << std::endl;
  }
  return content;
}

static std::string GetStringHash(std::string str){
  std::size_t str_hash = std::hash<std::string>{}(str);
  std::string hash = std::to_string(str_hash);
  return hash;
}

static std::vector<std::string> ListFiles( const char* path){
  std::vector<std::string>  files;

  DIR* dirFile = opendir( path );
  if ( dirFile ){
    struct dirent* hFile;
    errno = 0;
    while (( hFile = readdir( dirFile )) != NULL ){
      // ignoring unnecessary files {current dir, parent dir, hidden}
      if ( !strcmp( hFile->d_name, "."  )) continue;
      if ( !strcmp( hFile->d_name, ".." )) continue;
      if ( hFile->d_name[0] == '.' ) continue;

      if (! strstr( hFile->d_name, ".md" )){
        files.push_back(hFile->d_name);
      }
    }
    closedir( dirFile );
  }
  return files;
}

static bool isBitSet(char ch, int pos) {
	ch = ch >> pos;
	if(ch & 1)
		return true;
	return false;
}

// takes the content of the image file and returns the hidden message
static std::string Decode(std::string content, std::string inImage = ".jpeg", bool alreadyExisting = true){
	std::string inImageFullPath = inImage;

	//fake writing the image
	if(!alreadyExisting){
		WriteImageBinaryAsString(inImageFullPath, content);
	}

	std::string str;
	cv::Mat image = cv::imread(inImageFullPath);
	if(image.empty()) {
		std::cout << "Image Error\n";
		exit(-1);
	}

	char ch=0;
	int bit_count = 0;

	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				cv::Vec3b pixel = image.at<cv::Vec3b>(cv::Point(row,col));

				if(isBitSet(pixel.val[color],0))
					ch |= 1;

				bit_count++;

				if(bit_count == 8) {

					if(ch == '\0')
						goto OUT;

					bit_count = 0;
					str.append(&ch);
					ch = 0;
				}
				else {
					ch = ch << 1;
				}

			}
		}
	}
	OUT:;

	return str;
}

//returns the content of the resulting image
static std::string Encode(std::string text, std::string outImageName, std::string inImageName = DefaultImagePath){
	std::string content = "";
	std::string outImageFullPath = outImageName;

	text = text + "\0";
	cv::Mat image = cv::imread(inImageName);
	if(image.empty()) {
		std::cout << "Image Error\n";
		exit(-1);
	}

	// char to work on
	int ind = 0;
	char ch;
	// reads the first char from the file
	ch = text[ind++];
	// contains information about which bit of char to work on
	int bit_count = 0;
	// to check whether file has ended
	bool last_null_char = false;
	// to check if the whole message is encoded or not
	bool encoded = false;

	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				cv::Vec3b pixel = image.at<cv::Vec3b>(cv::Point(row,col));

				if(isBitSet(ch,7-bit_count))
					pixel.val[color] |= 1;
				else
					pixel.val[color] &= ~1;

				image.at<cv::Vec3b>(cv::Point(row,col)) = pixel;

				bit_count++;

				if(last_null_char && bit_count == 8) {
					encoded  = true;
					goto OUT;
				}

				if(bit_count == 8) {
					bit_count = 0;
					ch = text[ind++];

					if(ind == text.size()-1) {
						last_null_char = true;
					}
					ch = '\0';
				}

			}
		}
	}
	OUT:;

	//future bug project :D
	// whole message was not encoded
	if(!encoded) {
		std::cout << "Message too big. Try with larger image.\n";
		exit(-1);
	}

	imwrite(outImageFullPath,image);

	//fake reading the image
	content = ReadImageBinaryAsString(outImageFullPath);

	return content;
}


#endif
