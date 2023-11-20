
#ifndef SAVE_OBJECT_H
#define SAVE_OBJECT_H

#include <string>


    void saveEmail(const std::string& database_path, const std::string& username, const std::string& filename, const std::string& email_content);
    void saveFile(const std::string& file_path, const std::string& file_content);

#endif // SAVE_OBJECT_H