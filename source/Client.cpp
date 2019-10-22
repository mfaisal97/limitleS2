#include "../header/Client.h"

Client::Client(char * _hostname, int _port)
{
}


Client::~Client()
{
}

Message *  Client::execute(Message * _message)
{
  return new Message(0,0,0,0);
}
