#include "../header/Server.h"

Server::Server(char * _listen_hostname, int _listen_port){
  udpServerSocket = new UDPServerSocket(_listen_hostname, _listen_port);
  //cout <<"Started Server :\t\ton:\t" << _listen_port << "\n";
}

Server::~Server(){
  delete udpServerSocket;
}

Message *  Server::getRequest(){
  char* returned = new char[MAX_MESSAGE_SIZE];
  udpServerSocket->readFromSocketWithBlock(returned, MAX_MESSAGE_SIZE);
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
  // std::cout << "Sent this" << FromCharArray(_message->marshal()) <<std::endl;
  child->writeToSocket(_message->marshal(), -1);
  delete child;
}


void Server::serveRequest(){
  std::cout << "starting service\n";
  int serverReceived = 0;
  while (true){
    Message* m = doOperation(getRequest());
    //std::cout << "Server got message: \t" <<(char*) m->getMessage() << "\n";
    //child
    if(fork() == 0){
      sendReply(m);
      delete m;
      break;
    }
    //parent
    else {
      serverReceived++;
      char exitmessage[2]("q");
      if (strcmp( (char*) m->getMessage(), exitmessage ) == 0){
        std::cout << "okay man I am done.\t"<< serverReceived << std::endl;
        break;
      }
    }
  }


}
