#include "SMTPClient.h"


void SMTPClient::sendEmail(const Email& email) 
{

    if (!connectToServer()) {
        std::cerr << "Failed to connect to the SMTP server." << std::endl;
        return;
    }

    // Perform SMTP handshake
    sendCommand("EHLO [127.0.0.1] \r\n");
    receiveResponse();

    // Specify sender
    sendCommand("MAIL FROM: <" + email.getSender() + ">\r\n");
    receiveResponse();

    // Specify recipient
    sendCommand("RCPT TO: <" + email.getRecipient() + ">\r\n");
    receiveResponse();

    // Start email data
    sendCommand("DATA\r\n");
    receiveResponse();

    // Send email content
    // Send email content in chunks
    std::string emailContent = email.formatMail();
    const int chunk_size = 1024; // Choose an appropriate chunk size
    for (size_t i = 0; i < emailContent.length(); i += chunk_size) {
        sendCommand(emailContent.substr(i, chunk_size) + "\r\n");
        
    }

    
    sendCommand("\r\n.\r\n");

    // Quit session
    sendCommand("QUIT\r\n");
    receiveResponse();

    // Close Socket and WSA
    closesocket(client_socket);
    WSACleanup();
}



