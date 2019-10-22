Client::Client(char * _hostname, int _port)
{
  udpSocket = new UDPClientSocket(_hostname, _port);
  cout <<"Started Client to \t" << _hostname << "\t\ton:\t" << _port<< "\n\n";
}


Client::~Client()
{
  delete udpSocket;
}

Message *  Client::execute(Message * _message)
{
  udpSocket->writeToSocket(_message->habd, -1);
  int max_returned = 1024;
  char* returned = new char[max_returned];

  udpSocket->readFromSocketWithBlock(returned, max_returned);
  return new Message(returned);
}
