#include <arpa/inet.h>

Server::Server(char * _listen_hostname, int _listen_port){
  udpServerSocket = new UDPServerSocket(_listen_hostname, _listen_port);
  //cout <<"Started Server :\t\ton:\t" << _listen_port << "\n";
}


Server::~Server(){
  delete udpServerSocket;
}

Message *  Server::getRequest(){
  int max_returned = 1024;
  char* returned = new char[max_returned];
  udpServerSocket->readFromSocketWithBlock(returned, max_returned);
  return new Message(returned);
}

Message *  Server::doOperation(Message * message){
  return message;
}

void Server::sendReply (Message * _message){
  char str[INET_ADDRSTRLEN];
  struct sockaddr_in sa = udpServerSocket->getPeerAddr();
  inet_ntop(AF_INET,&(sa.sin_addr), str, INET_ADDRSTRLEN);
  UDPClientSocket * child = new UDPClientSocket (str, ntohs(sa.sin_port));
  //cout << "habd: " << str  << "\t"<< sa.sin_port << "\t" << m->habd << "\n";
  child->writeToSocket(_message->habd, -1);
}


void Server::serveRequest(){
  cout << "starting service\n";
  int serverReceived = 0;
  while (true){
    Message* m = doOperation(getRequest());
    cout << "Server got message: \t" << m->habd << "\n";
    //child
    if(fork() == 0){
      sendReply(m);
      break;
    }
    //parent
    else {
      serverReceived++;
      char exitmessage[2]("q");
      if (strcmp( m->habd, exitmessage ) == 0){
        cout << "okay man I am done.\t"<< serverReceived <<endl;
        break;
      }
    }
  }


}
