#include "SaveObject.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveEmail(const std::string& database_path, const std::string& username, const std::string& filename, const std::string& email_content)
{
    std::string file_path = database_path + "/" + username + "/" + filename;;
    // create folder
    if (!fs::exists(database_path + "/" + username))
    {
        fs::create_directory(database_path + "/" + username);
        
        // use for debug
        std::cout << "User folder created: " << username << "\n";
    }

    // save email
    std::ofstream file(file_path);

    if (file.is_open())
    {
        file << email_content;
        file.close();

        std::cout << "Email saved in folder: " << file_path << "\n   ";
    }
    else
    {
        std::cerr << "Error saving email in folder: " << file_path << "\n";
    }
}