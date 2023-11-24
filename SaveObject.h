
#ifndef SAVE_OBJECT_H
#define SAVE_OBJECT_H

#include <string>


    void saveEmail(const std::string& username, const std::string& email_content, const std::string& filename);
    void saveFile(const std::string& file_path, const std::string& file_content);

#endif // SAVE_OBJECT_H