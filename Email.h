#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "base64.h"

#ifndef EMAIL_H
#define EMAIL_H

class Email
{
private:
    std::string email_sender;
    std::string email_recipient;
    std::string subject;
    std::string message;
    std::vector<std::string> cc_list;
    std::vector<std::string> bcc_list;
    std::vector<std::string> attachment_list;

public:
    // constructor
    Email(const std::string& SENDER, const std::string& TO, const std::string& SUBJECT, const std::string& MSG)
        : email_sender(SENDER), email_recipient(TO), subject(SUBJECT), message(MSG) {}
    
    // add cc, bcc
    void addCc(const std::string& recipient);
    void addBcc(const std::string& recipient);

    // attach file
    void attachFile(const std::string& file_path);

    // return format of mail
    std::string formatMail() const;

    // getter 
    std::string getSender() const;
    std::string getRecipient() const;

};
    
#endif // EMAIL