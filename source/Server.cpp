#include "../header/Server.h"
#include "../header/Message.h"
Server::Server(char * _listen_hostname, int _listen_port)
{
}


Server::~Server()
{
}

Message *  Server::getRequest()
{
  return new Message;
}

Message *  Server::doOperation()
{
  return new Message;
}

void Server::sendReply (Message * _message){

}


void Server::serveRequest(){

}
