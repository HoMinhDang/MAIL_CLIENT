#include "POP3Client.h"

std::string POP3Client::receiveMultilineResponse() const
{
    std::string response;
    do{
        response += receiveResponse();

    }while(response.find("\r\n.\r\n") == std::string::npos);
    return response;
}

bool POP3Client::login(const std::string& username, const std::string& password)
{
    if (!connectToServer())
        return false;

    // username
    receiveResponse(); // first message
    sendCommand("USER " + username + "\r\n");
    receiveResponse();

    // password
    sendCommand("PASS " + password + "\r\n");
    std::string pass_response = receiveResponse(); // Receive response to PASS command

    return pass_response.find("+OK") != std::string::npos;
}

std::vector<std::string> POP3Client::listEmail()
{
    sendCommand("LIST\r\n");
    std::string list_response = receiveMultilineResponse(); // receive response

    std::vector<std::string> list_email;
    std::istringstream response_stream(list_response);
    std::string line;

    // skip first line
    std::getline(response_stream, line);

    while (std::getline(response_stream, line))
    {
        std::istringstream line_stream(line);
        std::string email_ID;
        line_stream >> email_ID;

        // check is this email in downloaded 
        if (downloaded_email.find(email_ID) == downloaded_email.end())
        {
            downloaded_email.insert(email_ID);
            list_email.push_back(email_ID);
        }
    }
    return list_email;
}

std::string POP3Client::retrieveEmail(int email_ID)
{
    sendCommand("RETR " + std::to_string(email_ID) + "\r\n");
    return receiveMultilineResponse();
}

std::string POP3Client::getMailBoxStatus()
{
    sendCommand("STAT\r\n");
    return receiveResponse();
}

bool POP3Client::markForDeletion(int email_ID)
{
    sendCommand("DELE " + std::to_string(email_ID) + "\r\n");
    std::string response = receiveResponse();
    return response.find("+OK") != std::string::npos;
}

bool POP3Client::resetSession()
{
    sendCommand("RSET\r\n");    
    std::string response = receiveResponse();
    return response.find("+OK") != std::string::npos;
}

void POP3Client::quitSession()
{
    sendCommand("QUIT\r\n");
    receiveResponse(); // Receive response to QUIT command
}