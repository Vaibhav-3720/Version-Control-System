#include "../include/Diff.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

void Diff::show()
{
    ifstream head(".mg/HEAD");

    int commitID = 0;
    head >> commitID;
    head.close();

    if (commitID == 0)
    {
        cout << "No commits found.\n";
        return;
    }

    string snapshotFolder =
        ".mg/commits/" + to_string(commitID) + "/snapshot";

    //----------------------------------------------------------
    // Compare committed files with working directory
    //----------------------------------------------------------

    for (const auto &entry : fs::recursive_directory_iterator(snapshotFolder))
    {
        if (!entry.is_regular_file())
            continue;

        string relative =
            fs::relative(entry.path(), snapshotFolder).string();

        string workingFile = relative;

        if (!fs::exists(workingFile))
        {
            cout << "\nDeleted File : " << relative << endl;
            continue;
        }

        ifstream oldFile(entry.path());
        ifstream newFile(workingFile);

        vector<string> oldLines;
        vector<string> newLines;

        string line;

        while (getline(oldFile, line))
            oldLines.push_back(line);

        while (getline(newFile, line))
            newLines.push_back(line);

        if (oldLines != newLines)
        {
            cout << "\nModified File : " << relative << endl;
            cout << "--------------------------\n";

            size_t n = max(oldLines.size(), newLines.size());

            for (size_t i = 0; i < n; i++)
            {
                if (i >= oldLines.size())
                {
                    cout << "+ " << newLines[i] << endl;
                }
                else if (i >= newLines.size())
                {
                    cout << "- " << oldLines[i] << endl;
                }
                else if (oldLines[i] != newLines[i])
                {
                    cout << "- " << oldLines[i] << endl;
                    cout << "+ " << newLines[i] << endl;
                }
            }
        }
    }

    //----------------------------------------------------------
    // Detect newly created files
    //----------------------------------------------------------

    for (const auto &entry : fs::recursive_directory_iterator("."))
    {
        if (!entry.is_regular_file())
            continue;

        string relative =
            fs::relative(entry.path(), fs::current_path()).string();

        // Ignore MiniGit repository
        if (relative.find(".mg") == 0)
            continue;

        fs::path committed =
            fs::path(snapshotFolder) / relative;

        if (!fs::exists(committed))
        {
            cout << "\nNew File : " << relative << endl;
        }
    }
}