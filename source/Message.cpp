Message::Message(OperationType _operation, void * p_message, size_t p_message_size, int p_rpc_id){
  operation = _operation;
  message = p_message;
  message_size = p_message_size;
  rpc = p_rpc_id;
  message_type = MessageType::Request;
}

Message::Message(char * _marshalled_base64) : Message(FromCharArray(_marshalled_base64)){
}

Message::Message(string _marshalled_base64){
  string str = _marshalled_base64;
  message_type = MessageType(GetNumberBetweenBracket(str));
  operation = OperationType(GetNumberBetweenBracket(str));
  rpc_id = GetNumberBetweenBracket(str);
  message_size = GetNumberBetweenBracket(str);
  message = str;
}

char * Message::marshal (){
  return ToCharArray(marshalString());
}

string Message::marshalString (){
  string str = "";
  str = str + NumberAsString(int(message_type));
  str = str + NumberAsString(int(operation));
  str = str + NumberAsString(int(rpc_id));
  str = str + NumberAsString(int(message_size));
  str = str + message;

  return str;
}


OperationType Message::getOperation (){
  return operation;
}

MessageType Message::getMessageType(){
  return message_type;
}

int Message::getRPCId(){
  return rpc_id;
}

void * Message::getMessage(){
  return message;
}

size_t Message::getMessageSize(){
  return message_size;
}

void Message::setOperation (OperationType _operation){
  operation = _operation;
}

void Message::setMessage (void * _message, size_t _message_size){
  message = _message;
  message_size = _message_size;
}

void Message::setMessageType (MessageType message_type){
  message_type = _message_type;
}

Message::~Message(){

}
