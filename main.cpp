#include <iostream>
#include "header/ServiceDirectory.h"
#include "header/Client.h"
#include <string>

using namespace std;

int main(int argc, char *argv[]){
  if (argc >= 3){

    //client testing
    if( argc >= 7 && strcmp( argv[1], "client") == 0 && strcmp( argv[4], "test") == 0){
      cout << "Starting client testing!\n";
      Client c (argv[2], std::stoi(argv[3]));

      int clientReceived = 0;
      int maxTest = stoi(argv[5]);

      string str = "test";
      char* strcharacters = ToCharArray(str);

      while (maxTest--){
        Message* m = c.execute(new Message(strcharacters));
        if(strcmp( m->habd, "")){
          clientReceived++;
        }
      }

      cout << "Replies received:\t" << clientReceived << "\n";
    }


    //client start
    else if( argc >= 5 && strcmp( argv[1], "client") == 0){
      Client c (argv[2], std::stoi(argv[3]));
      while (true){
        cout << "Please, enter a message to be sent to the server:\n";
        string str;
        getline (cin, str);
        char strcharacters[str.size() + 1];
        str.copy(strcharacters, str.size() + 1);
        strcharacters[str.size()] = '\0';
        //cout << "You entered:\n" << strcharacters << "\n";
        cout << "Got reply:\n";
        Message* m = c.execute(new Message(strcharacters));
        cout << m->habd;
        cout << "\n\n\n";
        char exitmessage[2]("q");

        if(strcmp(strcharacters,exitmessage) == 0){
          cout << "okay man I am done.\n";
          break;
        }
      }
    }

      //running servre as test
      else if(strcmp( argv[1], "server" ) == 0){

      ServiceDirectory s (std::stoi(argv[2]));
      s.serveRequest();
      //print server address

    }
    else {
    cout << "bas y baba! it is less than 4";
    }



  }else {
    cout << "bas y baba! it is less than 3";
  }
  cout << "\n";
  return 0;
}
