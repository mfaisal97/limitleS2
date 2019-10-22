#include "../header/Client.h"
#include "../header/Message.h"

Client::Client()
{
}


Client::~Client()
{
}

Message *  Client::execute(Message * _message)
{
  return new Message;
}
