#include "../header/Client.h"

Client::Client(char * _hostname, int _port){
  udpSocket = new UDPClientSocket(_hostname, _port);
  std::cout <<"Started Client to \t" << _hostname << "\t\ton:\t" << _port<< "\n\n";
}


Client::~Client(){
  delete udpSocket;
}

Message *  Client::execute(Message * _message){
  udpSocket->writeToSocket(_message->marshal(), -1);
  char* returned = new char[MAX_MESSAGE_SIZE];
  udpSocket->readFromSocketWithTimeout(returned, MAX_MESSAGE_SIZE, 15, 0);
  // std::cout <<"got this\t" << FromCharArray(returned);
  return new Message(returned);
}
