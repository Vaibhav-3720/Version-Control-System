#include "../include/Logger.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void Logger::printLog()
{
    ifstream head(".mg/HEAD");

    int lastCommit = 0;
    head >> lastCommit;
    head.close();

    if (lastCommit == 0)
    {
        cout << "No commits yet.\n";
        return;
    }

    for (int id = lastCommit; id >= 1; id--)
    {
        string metadata =
            ".mg/commits/" + to_string(id) + "/metadata.txt";

        ifstream meta(metadata);

        string line;

        string commitID;
        string hash;
        string author;
        string branch;
        string date;
        string files;
        string message;

        while (getline(meta, line))
        {
            size_t pos = line.find('=');

            if (pos == string::npos)
                continue;

            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);

            if (key == "id")
                commitID = value;
            else if (key == "hash")
                hash = value;
            else if (key == "author")
                author = value;
            else if (key == "branch")
                branch = value;
            else if (key == "date")
                date = value;
            else if (key == "files")
                files = value;
            else if (key == "message")
                message = value;
        }

        meta.close();

        cout << "=========================================================\n";
        cout << "Commit #" << commitID << '\n';
        cout << "=========================================================\n";

        cout << "Hash          : " << hash << '\n';
        cout << "Author        : " << author << '\n';
        cout << "Branch        : " << branch << '\n';
        cout << "Date          : " << date << '\n';
        cout << "Files Changed : " << files << "\n\n";

        cout << "Message\n";
        cout << "-------\n";
        cout << message << "\n\n";
    }
}