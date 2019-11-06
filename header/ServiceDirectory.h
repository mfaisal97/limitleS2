#ifndef ServiceDirectory_H
#define ServiceDirectory_H

#include "../header/Server.h"
#include "../header/Message.h"

class ServiceDirectory
{
    private:
      Server server;
    public:
        ServiceDirectory(int _listen_port);
        ~ServiceDirectory();
};

#include "../source/ServiceDirectory.cpp"
#endif // ServiceDirectory_H
