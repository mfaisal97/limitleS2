#include "../header/Server.h"

Server::Server(char * _listen_hostname, int _listen_port){
  udpServerSocket = new UDPServerSocket(_listen_hostname, _listen_port);
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

Message *  Server::doOperation(){
  return new Message(0,0,0,0);
}

void Server::sendReply (Message * _message){
  udpServerSocket->writeToSocket(_message->habd, -1);
}


void Server::serveRequest(){
  while (true){
    Message* m = getRequest();
    if (strcmp( m->habd, "q" ) == 0){
      break;
    }
    sendReply(m);
  }
}
