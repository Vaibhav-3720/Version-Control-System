#include "../include/Commit.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

namespace fs = std::filesystem;
using namespace std;

int Commit::getNextCommitID()
{
    ifstream fin(".mg/HEAD");

    int id = 0;
    fin >> id;
    fin.close();

    id++;

    ofstream fout(".mg/HEAD");
    fout << id;
    fout.close();

    return id;
}

void Commit::create(const string &message)
{
    ifstream fin(".mg/index");

    string line;

    if (!getline(fin, line))
    {
        cout << "Nothing to commit.\n";
        return;
    }

    fin.close();

    int id = getNextCommitID();

    string commitFolder = ".mg/commits/" + to_string(id);

    fs::create_directory(commitFolder);
    fs::create_directory(commitFolder + "/snapshot");

    //--------------------------------------------------
    // Count staged files
    //--------------------------------------------------

    int fileCount = 0;

    for (const auto &entry : fs::recursive_directory_iterator(".mg/staging"))
    {
        if (entry.is_regular_file())
            fileCount++;
    }

    //--------------------------------------------------
    // Current Date & Time
    //--------------------------------------------------

    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    //--------------------------------------------------
    // Metadata
    //--------------------------------------------------

    ofstream meta(commitFolder + "/metadata.txt");

    // Generate a short hash
    size_t hashValue = hash<string>{}(to_string(id) + message);

    stringstream ss;
    ss << hex << hashValue;

    string commitHash = ss.str().substr(0, 7);

    // Write metadata
    meta << "id=" << id << '\n';
    meta << "hash=" << commitHash << '\n';
    meta << "author=Vaibhav Gupta\n";
    meta << "branch=main\n";

    meta << "date="
         << put_time(localtime(&currentTime), "%d-%m-%Y %H:%M:%S")
         << '\n';

    meta << "files=" << fileCount << '\n';
    meta << "message=" << message << '\n';

    meta.close();

    //--------------------------------------------------
    // Build Complete Snapshot
    //--------------------------------------------------

    // If this isn't the first commit, copy the previous snapshot
    if (id > 1)
    {
        string previousSnapshot =
            ".mg/commits/" + to_string(id - 1) + "/snapshot";

        if (fs::exists(previousSnapshot))
        {
            fs::copy(
                previousSnapshot,
                commitFolder + "/snapshot",
                fs::copy_options::recursive |
                    fs::copy_options::overwrite_existing);
        }
    }

    // Overlay staged files onto the snapshot
    for (const auto &entry : fs::recursive_directory_iterator(".mg/staging"))
    {
        if (!entry.is_regular_file())
            continue;

        fs::path relative =
            fs::relative(entry.path(), ".mg/staging");

        fs::path destination =
            fs::path(commitFolder) /
            "snapshot" /
            relative;

        fs::create_directories(destination.parent_path());

        fs::copy_file(
            entry.path(),
            destination,
            fs::copy_options::overwrite_existing);
    }

    fs::remove_all(".mg/staging");
    fs::create_directory(".mg/staging");

    ofstream index(".mg/index", ios::trunc);
    index.close();

    cout << "Commit " << id << " created successfully.\n";
}