#include "SaveObject.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace fs = std::filesystem;

void saveEmail(const std::string& username, const std::string& email_content, const std::string& filename)
{
    std::string file_path = username + "/" + filename;

    if (!fs::exists(username))
    {
        fs::create_directory(username);

        // use for debug
        // std::cout << "User folder created: " << username << "\n";
    }

    std::ofstream file(file_path);

    if (file.is_open())
    {
        file << email_content;
        file.close();

        // debug
        // std::cout << "Email saved in folder: " << file_path << "\n";
    }
    else
    {
        // debug
        // std::cerr << "Error saving email in folder: " << file_path << "\n";
    }
}



