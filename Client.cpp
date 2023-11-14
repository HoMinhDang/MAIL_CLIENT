#include "Client.h"

Client::~Client()
{
    if (client_socket != INVALID_SOCKET)
    {
        closesocket(client_socket);
        WSACleanup();
    }
}

void Client::sendCommand(const std::string& command) const
{
    // send(SOCKET, *buffer_str, length, flag)
    // 0 is default
    send(client_socket, command.c_str(), command.size(), 0);
}

std::string Client::receiveResponse() const
{
    char buffer[1024];
    // recv(SOCKET, *buffer_str, length, flag)
    // return number of byte or -1 if failure
    int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0); 
    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        std::string response(buffer);
        displayResponse(response); // use when debug
        return response;
    }
    return "";
}

void Client::displayResponse(const std::string& response) const
{
    std::cout << "Server response: " << response << std::endl;
}


bool Client::connectToServer() 
{
    // Create Windows sockets API
    WSADATA wsa_data;   
    WORD w_version_requested = MAKEWORD(2, 2); // version 2.2 of Winsock
    // WSAStarup return 0 on success 
    if (WSAStartup(w_version_requested, &wsa_data) != 0)
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return false;
    }

    // Create socket
        // socket(domain, type, protocol)
        // domain is AF_INET: IPv4
        // type: SOCK_STREAM:  Establishes a virtual circuit for stream => TCP
        // protocol 0      
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket.";
        WSACleanup();
        return false;
    }

    // create Socket address for IPv4
    SOCKADDR_IN server_info;
    server_info.sin_family = AF_INET; // type = IPv4
    server_info.sin_addr.s_addr = inet_addr(address.c_str()); // inet_addr convert string representing IPv4 to binary form
    server_info.sin_port = htons(port); // converts a 16-bit host-byte-order integer to network-byte-order

    // connect to Server
    if (connect(client_socket, (sockaddr*)&server_info, sizeof(server_info)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to Server." << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return false;
    }

    receiveResponse(); // response the initial response from server
    return true;
}
