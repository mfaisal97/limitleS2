#include "../header/Message.h"

Message::Message(OperationType _operation, void * p_message, int p_message_size, int p_rpc_id){
  operation = _operation;
  message = p_message;
  message_size = p_message_size;
  rpc_id = p_rpc_id;
  message_type = MessageType::Request;
}

Message::Message(char * _marshalled_base64) : Message(FromCharArray(_marshalled_base64)){
}

Message::Message(std::string _marshalled_base64){
  std::string str = _marshalled_base64;
  message_type = (MessageType)GetNumberBetweenBracket(&str);
  operation = (OperationType)GetNumberBetweenBracket(&str);
  rpc_id = GetNumberBetweenBracket(&str);
  message_size = GetNumberBetweenBracket(&str);
  message = (void*) ToCharArray(str);
}

char * Message::marshal (){
  return ToCharArray(marshalString());
}

std::string Message::marshalString (){
  std::string str = "";
  str = str + NumberAsString(int(message_type));
  str = str + NumberAsString(int(operation));
  str = str + NumberAsString(int(rpc_id));
  str = str + NumberAsString(int(message_size));
  str = str + FromCharArray((char*) message);

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

int Message::getMessageSize(){
  return message_size;
}

void Message::setOperation (OperationType _operation){
  operation = _operation;
}

void Message::setMessage (void * _message, int _message_size){
  message = _message;
  message_size = _message_size;
}

void Message::setMessageType (MessageType _message_type){
  message_type = _message_type;
}

Message::~Message(){

}

// void DebugHere(Message* firstM,  Message* m){
//     // for debugging purposes
//     std::cout << "\nSent: \t" + firstM->marshalString() << "\n";
//     std::cout << "\nReceived: \t" + m->marshalString() << "\n";
//     std::cout << "Got reply Content:\n" << (char*) m->getMessage();
//     std::cout << "\n\n\n";
// }
