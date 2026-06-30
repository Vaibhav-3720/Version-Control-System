#include "../include/Utils.h"

#include <fstream>
#include <string>

using namespace std;

string Utils::getCommitHash(int commitID)
{
    string metadata =
        ".mg/commits/" + to_string(commitID) + "/metadata.txt";

    ifstream file(metadata);

    string line;

    while (getline(file, line))
    {
        if (line.rfind("hash=", 0) == 0)
        {
            return line.substr(5);
        }
    }

    return "Unknown";
}