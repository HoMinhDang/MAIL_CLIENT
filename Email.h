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
    std::string boundary;
    std::string message_ID;
    std::vector<std::string> cc_list;
    std::vector<std::string> bcc_list;
    std::vector<std::string> attachment_list;

public:
    // constructor
    Email(const std::string& SENDER, const std::string& TO, const std::string& SUBJECT, const std::string& MSG)
        : email_sender(SENDER), email_recipient(TO), subject(SUBJECT), message(MSG) {}
    
    // add cc, bcc, file_path
    void addCc(const std::string& recipient);
    void addBcc(const std::string& recipient);
    void attachFile(const std::string& file_path);

    // getter 
    std::string getSender() const;
    std::string getRecipient() const;
    std::string getCC() const;
    std::vector<std::string> getListCC() const;
    std::string getBCC() const;
    std::vector<std::string> getListBCC() const;

    // Handle file_path
    std::string getFilename(const std::string& file_path) const;
    std::string getFileExtension(const std::string& file_name) const;
    std::string getContentType(const std::string& file_extension) const;
    
    // Handle mail format 
    std::string getDomain() const;
    std::string genDate() const;
    std::string genUserAgent() const;
    std::string genUniqueString(int size_string) const;
    std::string genMessageID() const;
    std::string genBoundary() const;

    // return format of mail
    std::string formatMail() const;




};
    
#endif // EMAIL