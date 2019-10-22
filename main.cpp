#include <iostream>
#include "./header/Server.h"
#include "./header/Client.h"
using namespace std;
int main(int argc, char *argv[]){
  if (argc >= 3){
    if(strcmp( argv[1], "server" ) == 0){
      Server s ("", stoi(argv[2]));
      s.serveRequest();
      //print server address
    }else if( argc == 4 && strcmp( argv[1], "client")){
      Client c (argv[2], stoi(argv[3]));
    }else {
    cout << "bas y baba! it is less than 4"; 
    }
  }else {
    cout << "bas y baba! it is less than 3"; 
  }

  UDPServerSocket temp();
  std::cout<<"sds";
  int a;
  cin>>a;
  return 0; 
}
