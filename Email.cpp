#include "Email.h"

#include <map>
#include <ctime>
#include <iomanip>

const int FILE_SIZE_MAX = 3 * 1024 * 1024;

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
    // Check if file is larger than 3MB
    std::ifstream file(file_path, std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        std::streampos file_size = file.tellg();
        if (file_size > FILE_SIZE_MAX)
        {
            std::cerr << "File :" << file_path << " is larger than 3MB" << std::endl;
        }
        else
        {
            attachment_list.push_back(file_path);
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening file: " << file_path << std::endl;
    }
    
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

std::string Email::getDomain() const
{
    size_t atPos = email_sender.find("@");
    if (atPos != std::string::npos)
    {
        return email_sender.substr(atPos + 1);
    }
    return "";
}


std::string Email::genUniqueString(int size_string) const
{
    const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int size_charset = charset.size();

    std::ostringstream res;
    for (int i = 0; i < size_string; i++)
        res << charset[rand() % size_charset];
    return res.str();
}

std::string Email::genDate() const
{
    auto current_time = std::time(nullptr);
    current_time += (7 * 3600); // convert to GMT+7

    std::tm* GMT7_tm = std::gmtime(&current_time);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S +0700", GMT7_tm);

    return buffer;

}

std::string Email::genUserAgent() const
{
    return "NO NAME";
}

std::string Email::genMessageID() const 
{
    std::ostringstream ID;
    ID << "<" << genUniqueString(20) << "@" << getDomain()  << ">";
    return ID.str();
}

std::string Email::genBoundary() const
{
    std::ostringstream boundary;
    for (int i = 0; i < 10; i++)
        boundary << "-";
    boundary << genUniqueString(20);
    return boundary.str();
}


std::string Email::formatMail() const {
    std::ostringstream email_format;

    std::string boundary = genBoundary();

    // Check if there are attachments
    bool hasAttachments = !attachment_list.empty();

    if (hasAttachments) {
        email_format << "Content-Type: multipart/mixed; boundary=" << boundary << "\r\n";
        email_format << "Message-ID: " << genMessageID() << "\r\n";
        email_format << "Date: " << genDate() << "\r\n";
        email_format << "MIME-Version: 1.0\r\n";
        email_format << "User-Agent: " << genUserAgent() << "\r\n";
        email_format << "Content-Language: en-US\r\n";

        email_format << "From: " << email_sender << "\r\n";
        email_format << "To: " << email_recipient << "\r\n";
        email_format << "CC: " << getCC() << "\r\n";
        email_format << "BCC: " << getBCC() << "\r\n";
        email_format << "Subject: " << subject << "\r\n\r\n";
    } else {
        email_format << "Message-ID: " << genMessageID() << "\r\n";
        email_format << "Date: " << genDate() << "\r\n";
        email_format << "MIME-Version: 1.0\r\n";
        email_format << "User-Agent: " << genUserAgent() << "\r\n";
        email_format << "Content-Language: en-US\r\n";
        
        email_format << "From: " << email_sender << "\r\n";
        email_format << "To: " << email_recipient << "\r\n";
        email_format << "CC: " << getCC() << "\r\n";
        email_format << "BCC: " << getBCC() << "\r\n";
        email_format << "Subject: " << subject << "\r\n";
    }

    if (hasAttachments) {
        // Boundary for text/plain part
        email_format << "--" << boundary << "\r\n";
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

        email_format << "--" << boundary << "\r\n";

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
        email_format << "--" << boundary << "--" << "\r\n";
    }

    return email_format.str();
}



