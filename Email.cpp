#include "Email.h"
#include <map>


void Email::addCc(const std::string& recipient)
{
    cc_list.push_back(recipient);
}

void Email::addBcc(const std::string& recipient)
{
    bcc_list.push_back(recipient);
}

void Email::attachFile(const std::string& file_path)
{
    attachment_list.push_back(file_path);
}


std::string Email::getSender() const
{
    return email_sender;
}

std::string Email::getRecipient() const
{
    return email_recipient;
}

std::string Email::getCC() const 
{
    std::string res = "";
    if (!cc_list.empty())
    {
        for (const auto& recipient : cc_list)
            res += recipient + ", ";
        return res.substr(0, res.size() - 2);    
    }
    return res;    
}

std::string Email::getBCC() const
{
    std::string res = "";
    if (!bcc_list.empty())
    {
        for (const auto& recipient : cc_list)
            res += recipient + ", ";
        return res.substr(0, res.size() - 2);    
    }
    return res; 
}

std::string Email::getFilename(const std::string& file_path) const
{
    return file_path.substr(file_path.find_last_of("/\\") + 1);
}

std::string Email::getFileExtension(const std::string& file_name) const
{
    return file_name.substr(file_name.find_last_of(".") + 1);
}

std::string Email::getContentType(const std::string& file_extension) const 
{
    std::map<std::string, std::string> content_type_map = {
        {"txt", "text/plain"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"png", "image/png"},
        {"gif", "image/gif"},
        {"mp4", "video/mp4"},
        {"pdf", "application/pdf"},
        {"html", "text/html"},
        {"css", "text/css"},
        {"js", "application/javascript"},
        {"json", "application/json"},
        {"xml", "application/xml"},
        {"doc", "application/msword"},
        {"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
        {"xls", "application/vnd.ms-excel"},
        {"xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
        {"ppt", "application/vnd.ms-powerpoint"},
        {"pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
        {"zip", "application/zip"}
    };

    auto it = content_type_map.find(file_extension);
    if (it != content_type_map.end()) {
        return it->second;
    } else {
        return "application/octet-stream";
    }
}

std::string Email::getBoundary() const
{
    if (!attachment_list.empty())
        return "--boundary_delimiter\r\n";
    return "";
}

std::string Email::formatMail() const {
    std::ostringstream email_format;

    // Check if there are attachments
    bool hasAttachments = !attachment_list.empty();

    if (hasAttachments) {
        email_format << "Content-Type: multipart/mixed; boundary=boundary_delimiter\r\n";
        email_format << "MIME-Version: 1.0\r\n";
        email_format << "From: " << email_sender << "\r\n";
        email_format << "To: " << email_recipient << "\r\n";
        email_format << "CC: " << getCC() << "\r\n";
        email_format << "BCC: " << getBCC() << "\r\n";
        email_format << "Subject: " << subject << "\r\n\r\n";
    } else {
        email_format << "MIME-Version: 1.0\r\n";
        email_format << "From: " << email_sender << "\r\n";
        email_format << "To: " << email_recipient << "\r\n";
        email_format << "CC: " << getCC() << "\r\n";
        email_format << "BCC: " << getBCC() << "\r\n";
        email_format << "Subject: " << subject << "\r\n";
    }

    if (hasAttachments) {
        // Boundary for text/plain part
        email_format << "--boundary_delimiter\r\n";
        email_format << "Content-Type: text/plain; charset=utf-8\r\n\r\n";
        email_format << message << "\r\n\r\n";
    } else {
        // Single-part body
        email_format << "Content-Type: text/plain; charset=utf-8\r\n\r\n";
        email_format << message << "\r\n";
    }

    // Attachment(s)
    for (const auto& file_path : attachment_list) {
        std::cout << "Attempting to open file: " << file_path << std::endl;

        std::string file_name = getFilename(file_path);

        email_format << "--boundary_delimiter\r\n";

        // Determine content type based on file extension
        std::string file_extension = getFileExtension(file_name);
        std::string content_type = getContentType(file_extension);

        email_format << "Content-Type: " << content_type << "; name=" << file_name << "\r\n";
        email_format << "Content-Disposition: attachment; filename=" << file_name << "\r\n";
        email_format << "Content-Transfer-Encoding: base64\r\n\r\n";

        // Read and encode file content in base64
        std::ifstream file(file_path, std::ios::binary);
        if (file.is_open()) 
        {
            std::ostringstream file_content;
            file_content << file.rdbuf();
            file.close();

            std::string base64_content = base64_encode(reinterpret_cast<const unsigned char*>(file_content.str().data()), file_content.str().size());
            email_format << base64_content << "\r\n";
        } 
        else 
        {
            // Handle file opening error
            std::cerr << "Error opening file: " << file_path << std::endl;
        }
    }

    // End of email
    if (hasAttachments) {
        email_format << "--boundary_delimiter--\r\n";
    }

    return email_format.str();
}



