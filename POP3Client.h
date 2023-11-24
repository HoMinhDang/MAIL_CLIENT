#include "Client.h"
#include <set>
#include <vector>
#include <sstream>
#include "Email.h"


#ifndef POP3_CLIENT_H
#define POP3_CLIENT_H

class POP3Client : public Client
{
private:
    std::set<Email> downloaded_email;

public:
    POP3Client(const std::string& server_address, int server_port)
        : Client(server_address, server_port){}
    ~POP3Client(){}

    bool login(const std::string& username, const std::string& password);
    
    int getCountEmail() const;

};

#endif // POP3_CLIENT_H