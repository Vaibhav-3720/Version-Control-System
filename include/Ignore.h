#ifndef IGNORE_H
#define IGNORE_H

#include <string>
#include <vector>

class Ignore
{
private:
    std::vector<std::string> patterns;

public:
    Ignore();

    bool isIgnored(const std::string &path);
};

#endif