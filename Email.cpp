#include "Email.h"

const int CHUNK_SIZE = 1024;

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

std::string Email::formatMail() const {
    std::ostringstream email_format;

    // MIME Version
    email_format << "MIME-Version: 1.0\r\n";

    // Header
    email_format << "From: " << email_sender << "\r\n";
    email_format << "To: " << email_recipient << "\r\n";

    // CC
    if (!cc_list.empty()) {
        email_format << "CC: ";
        for (const auto& recipient : cc_list)
            email_format << recipient << " ";
        email_format << "\r\n";
    }

    // BCC
    if (!bcc_list.empty()) {
        email_format << "BCC: ";
        for (const auto& recipient : bcc_list)
            email_format << recipient << " ";
        email_format << "\r\n";
    }

    // Subject
    email_format << "Subject: " << subject << "\r\n";

    // MIME Header for multipart content
    email_format << "Content-Type: multipart/mixed; boundary=boundary_delimiter\r\n\r\n";

    // Body
    email_format << "--boundary_delimiter\r\n";
    email_format << "Content-Type: text/plain; charset=utf-8\r\n\r\n";
    email_format << message << "\r\n";

    // Attachment
    for (const auto& file_path : attachment_list) {
        std::cout << "Attempting to open file: " << file_path << std::endl;

        std::string file_name = file_path.substr(file_path.find_last_of("/\\") + 1); // Extract the file name from the path

        email_format << "--boundary_delimiter\r\n";

        // Determine content type based on file extension
        std::string file_extension = file_name.substr(file_name.find_last_of(".") + 1);
        std::string content_type;
        if (file_extension == "txt") {
            content_type = "text/plain";
        } else if (file_extension == "jpg" || file_extension == "jpeg") {
            content_type = "image/jpeg";
        } else if (file_extension == "png") {
            content_type = "image/png";
        } else if (file_extension == "gif") {
            content_type = "image/gif";
        } else if (file_extension == "mp4") {
            content_type = "video/mp4";
        } else if (file_extension == "pdf") {
            content_type = "application/pdf";
        } else {
            // Set a default content type for unknown file types
            content_type = "application/octet-stream";
        }

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
    email_format << "--boundary_delimiter--\r\n";

    return email_format.str();
}



std::string Email::getSender() const
{
    return email_sender;
}

std::string Email::getRecipient() const
{
    return email_recipient;
}