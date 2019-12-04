#ifndef UTILITIES_H
#define UTILITIES_H

#include "Constants.h"

// #include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <sys/stat.h>

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


static bool isBitSet(char ch, int pos);
static std::string encode(std::string message, std::string output_path, std::string ext , std::string input_path);
static std::string base64Decode(const char* Data, int DataByte);
static std::string base64Encode(const unsigned char* Data, int DataByte);
static std::string Mat2Base64(const cv::Mat &img, std::string imgType);
static cv::Mat Base2Mat(std::string &base64_data);
static std::string decode(std::string steg_base64);


static bool isBitSet(char ch, int pos) {
	ch = ch >> pos;
	if(ch & 1)
		return true;
	return false;
}

static std::string encode(std::string message, std::string output_path, std::string ext = "png", std::string input_path = defaultImagePath){
    std::string file_path = "message.txt";
    std::string output_file = output_path + "." + ext;
    std::string image_path = input_path;

		// std::cout << "Trying to encode \t" << message << std::endl;
		// std::cout << "In the following path \t" << output_file << std::endl;
		// std::cout << "With the following path as default\t" << input_path << std::endl;


    //store message in text file
    std::ofstream out(file_path);
    out << message;
    out.close();

	// Stores original image
	cv::Mat image = cv::imread(image_path);
	if(image.empty()) {
		std::cout << "Image Error\n";
		exit(-1);
	}

	// Open file for text information
	std::ifstream file(file_path);
	if(!file.is_open()) {
		std::cout << "File Error\n";
		exit(-1);
	}

	// char to work on
	char ch;
	// reads the first char from the file
	file.get(ch);
	// contains information about which bit of char to work on
	int bit_count = 0;
	// to check whether file has ended
	bool last_null_char = false;
	// to check if the whole message is encoded or not
	bool encoded = false;

	/*
	To hide text into images. We are taking one char (8 bits) and each of the 8 bits are stored
	in the Least Significant Bits (LSB) of the pixel values (Red,Green,Blue).
	We are manipulating bits in such way that changing LSB of the pixel values will not make a huge difference.
	The image will still look similiar to the naked eye.
	*/

	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {
				// stores the pixel details

				cv::Vec3b pixel = image.at<cv::Vec3b>(cv::Point(row,col));

				// if bit is 1 : change LSB of present color value to 1.
				// if bit is 0 : change LSB of present color value to 0.

				if(isBitSet(ch,7-bit_count))
					pixel.val[color] |= 1;
				else
					pixel.val[color] &= ~1;

				// update the image with the changed pixel values
				image.at<cv::Vec3b>(cv::Point(row,col)) = pixel;

				// increment bit_count to work on next bit
				bit_count++;


				// if last_null_char is true and bit_count is 8, then our message is successfully encode.
				if(last_null_char && bit_count == 8) {
					encoded  = true;
					goto OUT;
				}


				// if bit_count is 8 we pick the next char from the file and work on it


                if(bit_count == 8) {
					bit_count = 0;
					file.get(ch);


					// if EndOfFile(EOF) is encountered insert NULL char to the image
					if(file.eof()) {
						last_null_char = true;
						ch = '\0';
					}

				}

			}
		}
	}
	OUT:;

	// whole message was not encoded
	if(!encoded) {
		std::cout << "Message too big. Try with larger image.\n";
		exit(-1);
	}

	// Writes the stegnographic image
	cv::imwrite(output_file, image);
    return Mat2Base64(image, "png");

}


static std::string base64Decode(const char* Data, int DataByte)
{
	// Decoding table
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	 	//return value
	std::string strDecode;
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
				}
			}
			i += 4;
		}
		 else// carriage return, skip
		{
			Data++;
			i++;
		}
	}
	return strDecode;
}


static std::string base64Encode(const unsigned char* Data, int DataByte)
{
	 // code table
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	 	//return value
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	}
	 // Encode the remaining data
	int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}


	return strEncode;
}


static std::string Mat2Base64(const cv::Mat &img, std::string imgType)
{
	 //Mat to base64
	std::string img_data;
	std::vector<uchar> vecImg;
	std::vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imgType = "." + imgType;
	cv::imencode(imgType, img, vecImg, vecCompression_params);
	img_data = base64Encode(vecImg.data(), vecImg.size());
	return img_data;
}


static cv::Mat Base2Mat(std::string &base64_data)
{
	cv::Mat img;
	std::string s_mat;
	s_mat = base64Decode(base64_data.data(), base64_data.size());
	std::vector<char> base64_img(s_mat.begin(), s_mat.end());
	img = cv::imdecode(base64_img, CV_LOAD_IMAGE_COLOR);
	return img;
}



static std::string decode(std::string steg_base64) {
  cv::Mat image = Base2Mat(steg_base64);
	// std::cout << "\nRead following image\n" << steg_base64 << std::endl;
	// Stores original image
	// cv::Mat image = cv::imread("output.png");
	// if(image.empty()) {
	// 	std::cout << "Image Error\n";
	// 	exit(-1);
	// }

	// char to work on
	char ch=0;
	// contains information about which bit of char to work on
	int bit_count = 0;
	std::string decoded = "";

	/*
	To extract the message from the image, we will iterate through the pixels and extract the LSB of
	the pixel values (RGB) and this way we can get our message.
	*/
	for(int row=0; row < image.rows; row++) {
		for(int col=0; col < image.cols; col++) {
			for(int color=0; color < 3; color++) {

				// stores the pixel details
				cv::Vec3b pixel = image.at<cv::Vec3b>(cv::Point(row,col));

				// manipulate char bits according to the LSB of pixel values
				if(isBitSet(pixel.val[color],0))
					ch |= 1;

				// increment bit_count to work on next bit
				bit_count++;

				// bit_count is 8, that means we got our char from the encoded image
				if(bit_count == 8) {
					// std::cout << "hey" << std::endl;

					// NULL char is encountered
					if(ch == '\0'){
						// std::cout << "Reached end of file in decoding\t";
						goto OUT;
					}

					bit_count = 0;
					//std::cout << ch;
					decoded += ch;
					ch = 0;
				}
				else {
					ch = ch << 1;
				}

			}
		}
	}
	OUT:;

		// std::cout << "this rt:\t" << decoded.size() << std::endl;
    return decoded;
}




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
  // std::cout<< "parsing: \t" << *str <<std::endl;
  std::map<std::string, std::string> m;
  int sz = GetNumberBetweenBracket(str);
  while(sz--){
    std::string key = GetBetweenBrackets(str);
    std::string val = GetBetweenBrackets(str);
    m[key] = val;
  }
  // std::cout<< "After parsing: \t" << *str <<std::endl;
  return m;
}

static std::map<std::string, int> ParseIntMap(std::string * str){
  // std::cout<< "parsing: \t" << *str <<std::endl;
  std::map<std::string, int> m;
  int sz = GetNumberBetweenBracket(str);
  while(sz--){
    std::string key = GetBetweenBrackets(str);
    int val = GetNumberBetweenBracket(str);
    m[key] = val;
  }
  // std::cout<< "After parsing: \t" << *str <<std::endl;
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
		str += array[i];
		i++;
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



// 7agat SHehab beh
// 7agat SHehab beh
// 7agat SHehab beh
// 7agat SHehab beh
// 7agat SHehab beh
// 7agat SHehab beh
// 7agat SHehab beh
static bool createDirectoryWithFiles(std::string directoryName, int numberOfFiles, std::string message){
	//open directory
	int messageSize = message.size();
	if (mkdir(ToCharArray(directoryName), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
	{
		std::cout << "could not create directories\t" + directoryName << std::endl;
		return false;
	}
	else {

		//partition
		int j = 0;
		for (int i = 0; i < messageSize; i += messageSize/numberOfFiles){

			std::string str = message.substr(i, messageSize/numberOfFiles);
			encode(str, directoryName + '/' + std::to_string(j) );

			j++;
		}
	}
	return true;
}


static int getNumberOfFiles(int messageSize){
	cv::Mat image = cv::imread(defaultImagePath);
	if(image.empty()) {
		std::cout << "Image Error\n";
		exit(-1);
	}
	float maxNumberOfChars = image.cols * image.rows * 3 / 10;
	float numberOfFiles = messageSize / maxNumberOfChars;
	return ceil(numberOfFiles);

}

static std::string decodeAllInDirectory(std::string dirc, std::vector<std::string> files){
	std::string decoded = "";
	for(int it = 0; it < files.size(); ++it) {
		cv::Mat image = cv::imread(dirc + "/" + files[it]);
		if(image.empty()) {
			std::cout << "Image Error\n";
			exit(-1);
		}
		decoded += decode(Mat2Base64(image, "png"));
}
return decoded;
}

// 7agat SHehab beh
// 7agat SHehab beh
// 7agat SHehab beh




// static const std::string base64_chars =
//              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//              "abcdefghijklmnopqrstuvwxyz"
//              "0123456789+/";
//
//
// static inline bool is_base64(unsigned char c) {
//   return (isalnum(c) || (c == '+') || (c == '/'));
// }
//
// static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
//   std::string ret;
//   int i = 0;
//   int j = 0;
//   unsigned char char_array_3[3];
//   unsigned char char_array_4[4];
//
//   while (in_len--) {
//     char_array_3[i++] = *(bytes_to_encode++);
//     if (i == 3) {
//       char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
//       char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
//       char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
//       char_array_4[3] = char_array_3[2] & 0x3f;
//
//       for(i = 0; (i <4) ; i++)
//         ret += base64_chars[char_array_4[i]];
//       i = 0;
//     }
//   }
//
//   if (i)
//   {
//     for(j = i; j < 3; j++)
//       char_array_3[j] = '\0';
//
//     char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
//     char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
//     char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
//
//     for (j = 0; (j < i + 1); j++)
//       ret += base64_chars[char_array_4[j]];
//
//     while((i++ < 3))
//       ret += '=';
//
//   }
//
//   return ret;
//
// }
//
// static std::string base64_decode(std::string const& encoded_string) {
//   int in_len = encoded_string.size();
//   int i = 0;
//   int j = 0;
//   int in_ = 0;
//   unsigned char char_array_4[4], char_array_3[3];
//   std::string ret;
//
//   while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
//     char_array_4[i++] = encoded_string[in_]; in_++;
//     if (i ==4) {
//       for (i = 0; i <4; i++)
//         char_array_4[i] = base64_chars.find(char_array_4[i]);
//
//       char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
//       char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
//       char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];
//
//       for (i = 0; (i < 3); i++)
//         ret += char_array_3[i];
//       i = 0;
//     }
//   }
//
//   if (i) {
//     for (j = 0; j < i; j++)
//       char_array_4[j] = base64_chars.find(char_array_4[j]);
//
//     char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
//     char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
//
//     for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
//   }
//
//   return ret;
// }

static bool WriteImageBinaryAsString(std::string directoryPath, std::string imageName, std::string ext, std::string content){
		cv::Mat image = Base2Mat(content);
		cv::imwrite(directoryPath + "/" + imageName + "." + ext, image);

  // content = base64_decode(content);
  // std::ofstream wf(imageFullPath, std::ios::out | std::ios::binary);
	// if(!wf) {
	// 	std::cout << "Cannot open Fake \"" + imageFullPath + "\" for writing" << std::endl;
	// 	return false;
	// }
	// wf.write((char *) &content, sizeof(content));
	// wf.close();
  return true;
}

static std::string ReadImageBinaryAsString(std::string directoryPath, std::string imageName, std::string ext){
	//
  // std::string content = "";
  // std::ifstream rf(imageFullPath, std::ios::binary);
  // if (rf){
	//
  //   std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(rf), {});
	//
  //   // // get length of file:
  //   // rf.seekg (0, rf.end);
  //   // int length = rf.tellg();
  //   // rf.seekg (0, rf.beg);
  //   //
  //   // char * buffer = new char [length];
  //   // rf.read (buffer,length);
  //   content = FromCharArray(buffer);
  //   //std::cout << "buffer is\t" << buffer<<std::endl;
  //   // std::cout << "content is\t" << content<<std::endl;
	//
  //   rf.close();
  // }else {
  //   std::cout << "Cannot open \"" + imageFullPath + "\" for reading" << std::endl;
  // }
  // return base64_encode((unsigned char*)ToCharArray(content), content.size());

	cv::Mat image = cv::imread(directoryPath + "/" + imageName + "." + ext);
	if (image.empty()){

		std::cout << "Image Error: \t"  +directoryPath + "/" + imageName + "." + ext + "\n" ;
		exit(-1);
	}

	std::string stegImageStr = Mat2Base64(image, ext);
	return stegImageStr;
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

#endif
