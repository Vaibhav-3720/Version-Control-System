#ifndef STATUS_H
#define STATUS_H

class Status
{
private:
    void printStaged();
    void printModified();
    void printUntracked();
    void printDeleted();

public:
    void print();
};

#endif