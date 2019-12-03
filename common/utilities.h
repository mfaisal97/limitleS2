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

#include <iterator>
#include <sstream>


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
  return GetNumberBetweenBracket(str);
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
  std::string str = NumberAsString(m.size());
  for (std::map<std::string, std::string>::iterator it=m.begin(); it!=m.end(); ++it){
    str = str + "\n{" + it->first + "}\n{" + it->second + "}";
  }
  std::cout<< "Created: \t" << str <<std::endl;
  return str;
}

static std::string IntMapAsString(std::map<std::string, int> m){
  std::string str = NumberAsString(m.size());
  for (std::map<std::string, int>::iterator it=m.begin(); it!=m.end(); ++it){
    str = str + "\n{" + it->first + "}\n{" + std::to_string(it->second) + "}";
  }
  std::cout<< "Created: \t" << str <<std::endl;
  return str;
}

static std::map<std::string, std::string> ParseMap(std::string * str){
  std::cout<< "parsing: \t" << str <<std::endl;
  std::map<std::string, std::string> m;
  int sz = GetNumberBetweenBracket(str);
  while(sz--){
    std::string key = GetBetweenBrackets(str);
    std::string val = GetBetweenBrackets(str);
    m[key] = val;
  }
  std::cout<< "After parsing: \t" << str <<std::endl;
  return m;
}

static std::map<std::string, int> ParseIntMap(std::string * str){
  std::cout<< "parsing: \t" << str <<std::endl;
  std::map<std::string, int> m;
  int sz = GetNumberBetweenBracket(str);
  while(sz--){
    std::string key = GetBetweenBrackets(str);
    int val = GetNumberBetweenBracket(str);
    m[key] = val;
  }
  std::cout<< "After parsing: \t" << str <<std::endl;
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

static std::string FromCharArray(std::vector<unsigned char> buffer){
  std::string str = "";

  for (int i = 0; i < buffer.size(); ++i){
    char c[1];
    c[0] = buffer[i];
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



static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

static std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = 0; j < i; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

static bool WriteImageBinaryAsString(std::string imageFullPath, std::string content){
  content = base64_decode(content);
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

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(rf), {});

    // // get length of file:
    // rf.seekg (0, rf.end);
    // int length = rf.tellg();
    // rf.seekg (0, rf.beg);
    //
    // char * buffer = new char [length];
    // rf.read (buffer,length);
    content = FromCharArray(buffer);
    //std::cout << "buffer is\t" << buffer<<std::endl;
    // std::cout << "content is\t" << content<<std::endl;

    rf.close();
  }else {
    std::cout << "Cannot open \"" + imageFullPath + "\" for reading" << std::endl;
  }
  return base64_encode((unsigned char*)ToCharArray(content), content.size());
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

	std::string str = "";
	cv::Mat image = cv::imread(inImageFullPath);
	if(image.empty()) {
		std::cout << "Image Error\n";
		exit(-1);
	}

  // std::cout<< image <<std::endl;

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
          std::cout << "man do something \n";

					if(ch == '\0')
						goto OUT;

					bit_count = 0;
          char c = ch;
          std::string nstr=" ";
          nstr[0] = ch;
          str = str + nstr;
					// str.append(" ");
					ch = 0;
				}
				else {
					ch = ch << 1;
				}

			}
		}
	}
	OUT:;

  std::cout << "\n\n\n"<< str.size() <<"\n\n\nthis text decode\n\n\n\n\n\n" << std::endl;
  std::cout << "this text\t" << str << std::endl;

	return str;
}

//returns the content of the resulting image
static std::string Encode(std::string text, std::string outImageName, std::string inImageName = DefaultImagePath){
  std::cout << "\n\n\n"<< text.size() <<"\n\n\nthis text encode\n\n\n\n\n\n" << std::endl;
  std::cout << "this text\t" << text << std::endl;
  text = text +"\0";
	std::string content = "";
	std::string outImageFullPath = outImageName;

	// text = text + "\0";
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
        // std::cout << ind << std::endl;

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

          if(text[ind] != '\0'){
              ch = text[ind++];
          }

					if(ind >= text.size()-1) {
						last_null_char = true;
            ch = '\0';
					}
				}

        ind++;
			}
		}
	}
	OUT:;
  // std::cout << "finished encoding" <<std::endl;

	//future bug project :D
	// whole message was not encoded
	if(!encoded) {
		std::cout << "Message too big. Try with larger image.\n";
		exit(-1);
	}
  //cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
  //cv::imshow("image", image);
  //cv::waitKey(30);
  // std::cout<< image <<std::endl;
	cv::imwrite(outImageFullPath,image);

	//fake reading the image
	content = ReadImageBinaryAsString(outImageFullPath);

  // std::cout << "\n\n\n"<< content.size() <<"\n\n\nthis content encode\n\n\n\n\n\n" << std::endl;
  // std::cout << "content text\t" << content << std::endl;



	return content;
}


#endif
