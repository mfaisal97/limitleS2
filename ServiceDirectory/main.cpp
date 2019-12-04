#include <iostream>
#include "../header/Peer.h"
#include "../header/Client.h"
#include "../header/ServiceDirectory.h"
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
        if(strcmp((char*) m->getMessage(), "")){
          clientReceived++;
        }
      }

      cout << "Replies received:\t" << clientReceived << "\n";
    }


    //client start
    else if( argc >= 5 && strcmp( argv[1], "client") == 0){
      cout << "Started Creating StegImage"  << endl;
      Peer peer("mickey", "mouse", 4051, ToCharArray(ServiceDirectoryAddr), ServiceDirectoryPort);
      peer.SetAuthInfo("mickey", "mouse");
      peer.RemoteSignUp();
      peer.RemoteSignIn();
      // peer.RemoteSignOut();

      std::string testName = "01.png";
      StegImage imOne (testName, "mickey");

      cout << "Created StegImage"  << endl;

      imOne.saveStegImage();
      //
      cout << "Saved StegImage"  << endl;
      //

      cout << "Trying to load\t" << imOne.GetHash() << endl;
      StegImage imTwo (imOne.GetHash());
      imTwo.changePlainName("0" + testName);
      imTwo.savePlainImage();
      // imTwo.removePlainImage();
      // // cout << s <<endl;
      //
      // string s = encode("y wadddddd", "hey.png");
      // cout << decode(s);


      //im.savePlainImage();
      // Client c (argv[2], std::stoi(argv[3]));
      // while (true){
      //
      //   //getting input
      //   cout << "Please, enter a message to be sent to the server:\n";
      //   string str;
      //   getline (cin, str);
      //   char* strcharacters = ToCharArray(str);
      //
      //   cout << "Cannot construct a Message Object\n";
      //   // constructing a message for execution
      //   Message* firstM = new Message(OperationType::splitted, (void*)strcharacters, str.length(), 0);
      //
      //   // Doing the communication
      //   Message* m = c.execute(firstM);
      //
      //   // for debugging purposes
      //   cout << "\nSent: \t" + firstM->marshalString() << "\n";
      //   cout << "\nReceived: \t" + m->marshalString() << "\n";
      //   cout << "Got reply Content:\n" << (char*) m->getMessage();
      //   cout << "\n\n\n";
      //
      //   //checking exiting the program
      //   char exitmessage[2]("q");
      //   if(strcmp(strcharacters,exitmessage) == 0){
      //     cout << "okay man I am done.\n";
      //     break;
      //   }
      // }
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
