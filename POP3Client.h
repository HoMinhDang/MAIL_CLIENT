#include "Client.h"
#include <set>
#include <vector>
#include <sstream>


#ifndef POP3_CLIENT_H
#define POP3_CLIENT_H

class POP3Client : public Client
{
private:
    std::set<std::string> downloaded_email;

    //recive multiline response
    std::string receiveMultilineResponse() const;

public:
    POP3Client(const std::string& server_address, int server_port)
        : Client(server_address, server_port){}
    ~POP3Client(){}

    // log in to POP3 server
    bool login(const std::string& username, const std::string& password);

    // list emails on the server
    std::vector<std::string> listEmail();

    // retrieve Email
    std::string retrieveEmail(int email_ID);

    // get status of mailbox
    std::string getMailBoxStatus();

    // mark email for deletion 
    bool markForDeletion(int email_ID);

    // reset session
    bool resetSession();

    //  quit session
    void quitSession();

    // load and save set downloaded mail
    void loadDownloadedEmails();
    void saveDownloadedEmails(const std::set<std::string>& downloadedEmails);

};



#endif // POP3_CLIENT_H