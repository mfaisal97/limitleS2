#include "../header/Server.h"

Server::Server(char * _listen_hostname, int _listen_port){
  
}


Server::~Server(){
}

Message *  Server::getRequest(){
  return new Message(0,0,0,0);
}

Message *  Server::doOperation(){
  return new Message(0,0,0,0);
}

void Server::sendReply (Message * _message){

}


void Server::serveRequest(){

}
