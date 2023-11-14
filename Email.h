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
    // getter 
    std::string getSender() const;
    std::string getRecipient() const;

    std::string getBoundary() const;
    std::string getCC() const;
    std::string getBCC() const;

    std::string getFilename(const std::string& file_path) const;
    std::string getFileExtension(const std::string& file_name) const;
    std::string getContentType(const std::string& file_extension) const;
    

    // return format of mail
    std::string formatMail() const;




};
    
#endif // EMAIL