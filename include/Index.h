#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <vector>

class Index
{
public:
    void addFile(const std::string &filename);
    void addAllFiles();
    void removeFile(const std::string &filename);

    void clear();

    std::vector<std::string> getFiles();
    
};

#endif