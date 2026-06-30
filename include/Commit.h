#ifndef COMMIT_H
#define COMMIT_H

#include <string>

class Commit
{
public:
    int getNextCommitID();
    void create(const std::string &message);
};

#endif