#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

class FileManager
{
public:
    bool exists(const std::string &filename);
    bool copyToStage(const std::string &filename);
};

#endif