#include "Client.h"
#include <set>
#include <vector>
#include <sstream>
#include "Email.h"
#include <filesystem>


#ifndef POP3_CLIENT_H
#define POP3_CLIENT_H

class POP3Client : public Client
{
private:
    std::set<std::string> downloaded_email;
    std::vector<std::string> list_uid;
    int count_email;

    std::string receiveResponsePOP3() const;

    void getCountEmail();
    void getListUID();

    std::string retrieveEmail(int email_number);

public:
    std::vector<std::string> list_email;

    POP3Client(const std::string& server_address, int server_port)
        : Client(server_address, server_port){}
    ~POP3Client(){}

    bool login(const std::string& username, const std::string& password);

    void retrieveAllEmail();

    void loadDownloadedEmail(const std::string& username);
    void saveDownloadEmail(const std::string& username);

};

#endif // POP3_CLIENT_H