#include "../include/Status.h"
#include "../include/Index.h"
#include "../include/Utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

void Status::print()
{
    cout << "Repository Status\n";
    cout << "=================\n\n";

    ifstream head(".mg/HEAD");

    int commitID = 0;

    if (head)
        head >> commitID;

    head.close();

    cout << "Branch : main\n";
    if (commitID == 0)
    {
        cout << "HEAD   : None\n\n";
    }
    else
    {
        cout << "HEAD   : "
             << Utils::getCommitHash(commitID)
             << "\n\n";
    }

    printStaged();
    printModified();
    printUntracked();
    printDeleted();
}

////////////////////////////////////////////////////////

void Status::printStaged()
{
    cout << "Staged Files\n";
    cout << "------------\n";

    Index index;

    vector<string> files = index.getFiles();

    if (files.empty())
    {
        cout << "No files staged.\n";
    }
    else
    {
        for (const auto &file : files)
            cout << file << endl;

        cout << "\nTotal Staged Files : "
             << files.size() << endl;
    }

    cout << endl;
}

////////////////////////////////////////////////////////

void Status::printModified()
{
    cout << "Modified Files\n";
    cout << "--------------\n";

    ifstream head(".mg/HEAD");

    int commitID = 0;
    head >> commitID;
    head.close();

    if (commitID == 0)
    {
        cout << "None\n\n";
        return;
    }

    string snapshot =
        ".mg/commits/" + to_string(commitID) + "/snapshot";

    bool found = false;

    for (const auto &entry : fs::recursive_directory_iterator(snapshot))
    {
        if (!entry.is_regular_file())
            continue;

        string relative =
            fs::relative(entry.path(), snapshot).string();

        if (!fs::exists(relative))
            continue;

        ifstream oldFile(entry.path());
        ifstream newFile(relative);

        vector<string> oldLines;
        vector<string> newLines;

        string line;

        while (getline(oldFile, line))
            oldLines.push_back(line);

        while (getline(newFile, line))
            newLines.push_back(line);

        if (oldLines != newLines)
        {
            cout << relative << endl;
            found = true;
        }
    }

    if (!found)
        cout << "None";

    cout << "\n\n";
}

////////////////////////////////////////////////////////

void Status::printUntracked()
{
    cout << "Untracked Files\n";
    cout << "---------------\n";

    ifstream head(".mg/HEAD");

    int commitID = 0;
    head >> commitID;
    head.close();

    set<string> tracked;

    if (commitID > 0)
    {
        string snapshot =
            ".mg/commits/" + to_string(commitID) + "/snapshot";

        for (const auto &entry : fs::recursive_directory_iterator(snapshot))
        {
            if (!entry.is_regular_file())
                continue;

            tracked.insert(
                fs::relative(entry.path(), snapshot).string());
        }
    }

    Index index;

    for (const auto &f : index.getFiles())
        tracked.insert(f);

    bool found = false;

    for (const auto &entry : fs::recursive_directory_iterator("."))
    {
        if (!entry.is_regular_file())
            continue;

        string relative =
            fs::relative(entry.path(), fs::current_path()).string();

        if (relative.rfind(".mg", 0) == 0)
            continue;

        if (tracked.count(relative))
            continue;

        cout << relative << endl;
        found = true;
    }

    if (!found)
        cout << "None";

    cout << "\n\n";
}

////////////////////////////////////////////////////////

void Status::printDeleted()
{
    cout << "Deleted Files\n";
    cout << "-------------\n";

    ifstream head(".mg/HEAD");

    int commitID = 0;
    head >> commitID;
    head.close();

    if (commitID == 0)
    {
        cout << "None\n";
        return;
    }

    string snapshot =
        ".mg/commits/" + to_string(commitID) + "/snapshot";

    bool found = false;

    for (const auto &entry : fs::recursive_directory_iterator(snapshot))
    {
        if (!entry.is_regular_file())
            continue;

        string relative =
            fs::relative(entry.path(), snapshot).string();

        if (!fs::exists(relative))
        {
            cout << relative << endl;
            found = true;
        }
    }

    if (!found)
        cout << "None";

    cout << endl;
}