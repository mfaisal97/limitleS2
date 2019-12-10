#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "../common/utilities.h"
// #include "../common/common.h"

enum MessageType { Request, Reply};
enum OperationType {
  SignUp, SignIn, SignOut,HandleReducedImage,
  UpdateInfo, GetOnline, SearchViewables,
  GetViewables, UpdateImage, GetPartNumber,
  GetNumberOfParts,GetNotification,
  splitted};

class Message
{
    private:
        MessageType message_type;
        OperationType operation;
        void * message;
        int message_size;
        int rpc_id;
    public:
        Message(OperationType _operation, void * p_message, int p_message_size, int p_rpc_id);
        Message(char * _marshalled_base64);
        Message(std::string _marshalled_base64);

        char * marshal ();
        std::string marshalString ();

        OperationType getOperation ();
        int getRPCId();
        void * getMessage();
        int getMessageSize();
        MessageType getMessageType();

        void setOperation (OperationType _operation);
        void setMessage (void * _message, int _message_size);
        void setMessageType (MessageType _message_type);
        ~Message();
};

//#include "../source/Message.cpp"
#endif // MESSAGE_H
