#include "base64.h"
#ifndef SAVE_OBJECT_H
#define SAVE_OBJECT_H

#include <string>


    void saveEmail(const std::string& username, const std::string& email_content, const std::string& filename, const std::string& subfolder);
    void saveFile(const std::string& file_path, const std::string& filename, const std::string& file_content);

#endif // SAVE_OBJECT_H