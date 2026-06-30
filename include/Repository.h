#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <string>

class Repository
{
public:
    void init();

    void add(const std::string &filename);

    void commit(const std::string &message);

    void log();

    void checkout(int commitID);

    void status();

    void reset();

    void rm(const std::string &filename);
    void diff();
    void graph();
};

#endif