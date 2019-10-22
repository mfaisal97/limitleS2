#include <iostream>
#include "header/Server.h"
#include "header/Client.h"
#include "string"

using namespace std;

int main(int argc, char *argv[]){
  if (argc >= 3){

    if(strcmp( argv[1], "server" ) == 0){

      Server s ("", stoi(argv[2]));
      s.serveRequest();
      //print server address

    }else if( argc >= 4 && strcmp( argv[1], "client")){
      Client c (argv[2], stoi(argv[3]));
      while (true){
        cout << "Please, enter a message to be sent to the server:\n";
        string str;
        cin >> str;
        char strcharacters[str.size() + 1];
        str.copy(strcharacters, str.size() + 1);
        strcharacters[str.size()] = '\0';
        cout << "You entered:\n" << strcharacters;
        cout << "Got reply:\n";
        Message* m = c.execute(new Message(strcharacters));
        cout << m->habd;
        cout << "\n\n\n";

        if(strcmp(strcharacters,"q") == 0){
          break;
        }
      }

    }else {
    cout << "bas y baba! it is less than 4"; 
    }

  }else {
    cout << "bas y baba! it is less than 3"; 
  }
  cout << "\n";
  return 0; 
}
