#ifndef UTILITIES
#define UTILITIES

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <functional>
#include <map>

using namespace std;

void setBroadcast(int s){
    int arg;
    #ifdef SO_BROADCAST
    arg =1;
    if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &arg, sizeof(arg)) <0){
        perror("setsockopt SO_BROADCAST---");
        exit(-1);
    }
    #endif
}

void makeLocalSA(struct sockaddr_in *sa){
    sa->sin_family = AF_INET;
    sa->sin_port = htons(0);
    sa-> sin_addr.s_addr = htonl(INADDR_ANY);
}
void makeDestSA(struct sockaddr_in * sa, char *hostname, int port){ struct hostent *host;
    sa->sin_family = AF_INET;
    if((host = gethostbyname(hostname))== NULL){
        printf("Unknown host name\n");
        exit(-1);
    }
    sa-> sin_addr = *(struct in_addr *) (host->h_addr);
    sa->sin_port = htons(port);
}

void makeReceiverSA(struct sockaddr_in *sa, int port){
    sa->sin_family = AF_INET;
    sa->sin_port = htons(port);
    sa-> sin_addr.s_addr = htonl(INADDR_ANY);
}

string GetBetweenBrackets(string* str){
  int start = str->find("{");
  int end = str->find("}");
  string rt = "";

  if (end >= 0 && start >= 0 && end - start >= 2){
      rt = str->substr(start + 1, end - start -1);
      *str = str->substr(end+1);
  }

  return rt;
}

int GetNumberBetweenBracket(string* str){
  string numStr = GetBetweenBrackets(str);
  return stoi(numStr);
}

bool GetBoolBetweenBracket(string* str){
  string numStr = GetBetweenBrackets(str);
  return stoi(numStr);
}
string StringAsString(string str){
  return "\n{" + str + "}";
}

string BoolAsString(bool x){
  return x?"\n{1}" : "\n{0}";
}

string NumberAsString(int n){
  return "\n{" + to_string(n) + "}";
}

string MapAsString(map<string, string> m){
  string str = "";
  for (std::map<string, string>::iterator it=m.begin(); it!=m.end(); ++it){
    str = str + "\n{" + it->first + "}\n{" + it->second + "}";
  }
  return str;
}

string IntMapAsString(map<string, int> m){
  string str = "";
  for (std::map<string, int>::iterator it=m.begin(); it!=m.end(); ++it){
    str = str + "\n{" + it->first + "}\n{" + to_string(it->second) + "}";
  }
  return str;
}

map<string, string> ParseMap(string * str){
  map<string, string> m;
  while(str->length()>0){
    string key = GetBetweenBrackets(str);
    string val = GetBetweenBrackets(str);
    m[key] = val;
  }
  return m;
}

map<string, int> ParseIntMap(string * str){
  map<string, int> m;
  while(str->length()>0){
    string key = GetBetweenBrackets(str);
    int val = GetNumberBetweenBracket(str);
    m[key] = val;
  }
  return m;
}

char* ToCharArray(string str){
  char* strcharacters = new char[str.size() + 1];
  str.copy(strcharacters, str.size() + 1);
  strcharacters[str.size()] = '\0';

  return strcharacters;
}

string FromCharArray(char* array){
  string str = "";
  int i = 0;

  while (array[i] != '\0'){
    char c[1];
    c[0] = array[i++];
    str.append(&c[0]);
  }

  return str;
}

bool ValidString(string str){
  return !(str.find('{') != std::string::npos || str.find('}') != std::string::npos || str.size() == 0);
}

bool ValidUserNameString(string name){
  return (!any_of(name.begin(), name.end(), ::isdigit)) && ValidString(name);
}

bool WriteImageBinaryAsString(string imageFullPath, string content){
  ofstream wf(imageFullPath, ios::out | ios::binary);
	if(!wf) {
		cout << "Cannot open Fake \"" + imageFullPath + "\" for writing" << endl;
		return false;
	}
	wf.write((char *) &content, sizeof(content));
	wf.close();
  return true;
}

string ReadImageBinaryAsString(string imageFullPath){
  string content = "";
  ifstream rf(imageFullPath, ios::binary);
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
    cout << "Cannot open \"" + imageFullPath + "\" for reading" << endl;
  }
  return content;
}

string GetStringHash(string str){
  std::hash<std::string> str_hash;
  string hash = str_hash(str);
  return hash;
}

vector<string> ListFiles( const char* path){
  vector<string>  files;

  DIR* dirFile = opendir( path );
  if ( dirFile ){
    struct dirent* hFile;
    errno = 0;
    while (( hFile = readdir( dirFile )) != NULL ){
      // ignoring unnecessary files {current dir, parent dir, hidden}
      if ( !strcmp( hFile->d_name, "."  )) continue;
      if ( !strcmp( hFile->d_name, ".." )) continue;
      if ( gIgnoreHidden && ( hFile->d_name[0] == '.' )) continue;

      if (! strstr( hFile->d_name, ".md" )){
        files.push_back(hFile->d_name);
      }
    }
    closedir( dirFile );
  }
  return files;
}

#endif
