#include "SaveObject.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace fs = std::filesystem;

void saveEmail(const std::string& username, const std::string& email_content, const std::string& filename)
{
    std::string database_folder = "database";
    std::string user_folder = database_folder + "/" + username;
    std::string file_path = user_folder + "/" + filename;

    if (!fs::exists(database_folder))
    {
        fs::create_directory(database_folder);
    }

    if (!fs::exists(user_folder))
    {
        fs::create_directory(user_folder);
    }

    std::ofstream file(file_path);

    if (file.is_open())
    {
        file << email_content;
        file.close();
    }
    else
    {
        std::cerr << "Error saving email in folder: " << file_path << "\n";
    }
}




