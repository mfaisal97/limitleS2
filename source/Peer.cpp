Peer::Peer(string name, string password, int port): Server("", port){
  cout << "Starting a new peer node on port:\t\t" << port << endl;
}

Peer::~Peer(){
}


Message *  Peer::doOperation(Message * message){
  return new Message("y wad");
}
