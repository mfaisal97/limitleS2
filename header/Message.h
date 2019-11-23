#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#define size_t int

enum MessageType { Request, Reply};
enum OperationType {
  SignUp, SignIn, SignOut,
  UpdateInfo, GetOnline, SearchViewables,
  GetViewables, UpdateImage,
  splitted};

class Message
{
    private:
        MessageType message_type;
        OperationType operation;
        void * message;
        size_t message_size;
        int rpc_id;
    public:
        Message(OperationType _operation, void * p_message, size_t p_message_size, int p_rpc_id);
        Message(char * _marshalled_base64);
        Message(string _marshalled_base64);

        char * marshal ();
        string marshalString ();

        OperationType getOperation ();
        int getRPCId();
        void * getMessage();
        size_t getMessageSize();
        MessageType getMessageType();

        void setOperation (OperationType _operation);
        void setMessage (void * _message, size_t _message_size);
        void setMessageType (MessageType _message_type);
        ~Message();
};

#include "../source/Message.cpp"
#endif // MESSAGE_H
