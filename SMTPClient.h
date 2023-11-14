#include "Email.h"
#include "Client.h"

#pragma comment(lib, "ws2_32.lib")


#ifndef SMTP_CLIENT_H
#define SMTP_CLIENT_H

class SMTPClient : public Client
{

public:
    SMTPClient(const std::string& server_address, int server_port)
        : Client(server_address, server_port){}
    ~SMTPClient(){}

    void sendEmail(const Email& email) ;
};



#endif // SMTP_CLIENT_H