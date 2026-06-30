#include "../include/Graph.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void Graph::show()
{
    ifstream head(".mg/HEAD");

    int lastCommit = 0;
    head >> lastCommit;
    head.close();

    if (lastCommit == 0)
    {
        cout << "No commits found.\n";
        return;
    }

    cout << "\nCommit Graph\n";
    cout << "============\n\n";

    for (int id = lastCommit; id >= 1; id--)
    {
        string metadata =
            ".mg/commits/" + to_string(id) + "/metadata.txt";

        ifstream meta(metadata);

        string hash;
        string author;
        string date;
        string message;

        string line;

        while (getline(meta, line))
        {
            if (line.rfind("hash=", 0) == 0)
                hash = line.substr(5);

            else if (line.rfind("author=", 0) == 0)
                author = line.substr(7);

            else if (line.rfind("date=", 0) == 0)
                date = line.substr(5);

            else if (line.rfind("message=", 0) == 0)
                message = line.substr(8);
        }

        meta.close();

        cout << "* " << hash;

        if (id == lastCommit)
            cout << " (HEAD -> main)";

        cout << '\n';

        cout << "| Author : " << author << '\n';
        cout << "| Date   : " << date << '\n';
        cout << "| Message: " << message << '\n';

        if (id != 1)
            cout << "|\n";
    }
}