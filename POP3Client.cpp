#include "POP3Client.h"


bool POP3Client::login(const std::string& username, const std::string& password)
{
    // Connect to the server
    if (!connectToServer())
    {
        std::cerr << "Failed to connect to the server.\n";
        return false;
    }

    // Send the USER command
    sendCommand("USER " + username + "\r\n");
    std::string userResponse = receiveResponse();  // Receive and store the response
    if (userResponse.substr(0, 3) != "+OK")
    {
        std::cerr << "Failed to send USER command\n";
        return false;
    }

    // Send the PASS command
    sendCommand("PASS " + password + "\r\n");
    std::string passResponse = receiveResponse();  // Receive and store the response
    if (passResponse.substr(0, 3) != "+OK")
    {
        std::cerr << "Failed to login\n";
        return false;
    }

    return true;
}

int POP3Client::getCountEmail() const
{
    // assume that is being connect to server 
    sendCommand("STAT\r\n"); 

    std::string stat_response = receiveResponse();
    std::istringstream iss(stat_response);
    std::string status;
    int count_emails = 0;
    iss >> status >> count_emails;
    return count_emails;
}

