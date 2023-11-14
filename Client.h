#include <iostream>
#include <string>
#include <winsock2.h>
#include "base64.h"

#pragma comment(lib, "ws2_32.lib")

#ifndef CLIENT_H
#define CLIENT_H

class Client
{
protected:
    std::string address;
    int port;
    SOCKET client_socket;

    // send command
    void sendCommand(const std::string& command) const;

    // receive response
    std::string receiveResponse() const;

    // display response
    void displayResponse(const std::string& response) const;

public:
    Client(const std::string& server_address, int server_port)
        : address(server_address), port(server_port){}
    virtual ~Client();

    // connect to server
    bool connectToServer() ;
};


#endif // CLIENT_H