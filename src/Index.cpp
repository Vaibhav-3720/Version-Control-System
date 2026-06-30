#include "../include/Index.h"
#include "../include/FileManager.h"
#include "../include/Ignore.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

/////////////////////////////////////////////////////
// Add File
/////////////////////////////////////////////////////

void Index::addFile(const string &filename)
{
    FileManager fm;

    // Check if file exists
    if (!fm.exists(filename))
    {
        cout << "File doesn't exist.\n";
        return;
    }

    // Prevent duplicate staging
    vector<string> files = getFiles();

    for (const auto &file : files)
    {
        if (file == filename)
        {
            cout << "File already staged.\n";
            return;
        }
    }

    // Copy file to staging area
    if (!fm.copyToStage(filename))
    {
        cout << "Failed to stage file.\n";
        return;
    }

    // Add filename to index
    ofstream index(".mg/index", ios::app);

    if (!index)
    {
        cout << "Failed to open index.\n";
        return;
    }

    index << filename << '\n';

    index.close();

    cout << "File staged successfully.\n";
}

/////////////////////////////////////////////////////
// Remove File
/////////////////////////////////////////////////////

void Index::removeFile(const string &filename)
{
    if (!fs::exists(".mg"))
    {
        cout << "Not a MiniGit repository.\n";
        return;
    }

    bool found = false;

    try
    {
        // Remove staged copy
        string stagedPath = ".mg/staging/" + filename;

        if (fs::exists(stagedPath))
        {
            fs::remove(stagedPath);
            found = true;
        }

        // Read current index
        ifstream fin(".mg/index");

        vector<string> files;
        string file;

        while (getline(fin, file))
        {
            if (file != filename)
                files.push_back(file);
            else
                found = true;
        }

        fin.close();

        // Rewrite index
        ofstream fout(".mg/index", ios::trunc);

        for (const auto &f : files)
        {
            fout << f << '\n';
        }

        fout.close();

        if (found)
            cout << filename << " unstaged successfully.\n";
        else
            cout << filename << " is not staged.\n";
    }
    catch (const fs::filesystem_error &e)
    {
        cout << "Filesystem Error: " << e.what() << endl;
    }
}

/////////////////////////////////////////////////////
// Clear Staging Area
/////////////////////////////////////////////////////

void Index::clear()
{
    if (!fs::exists(".mg"))
    {
        cout << "Not a MiniGit repository.\n";
        return;
    }

    if (fs::exists(".mg/staging"))
    {
        for (const auto &entry : fs::directory_iterator(".mg/staging"))
        {
            fs::remove_all(entry.path());
        }
    }

    ofstream index(".mg/index", ios::trunc);
    index.close();

    cout << "Staging area cleared.\n";
}

/////////////////////////////////////////////////////
// Get Staged Files
/////////////////////////////////////////////////////

vector<string> Index::getFiles()
{
    vector<string> files;

    ifstream fin(".mg/index");

    if (!fin)
        return files;

    string file;

    while (getline(fin, file))
    {
        files.push_back(file);
    }

    fin.close();

    return files;
}
void Index::addAllFiles()
{
    Ignore ignore;

    fs::recursive_directory_iterator it(".");
    fs::recursive_directory_iterator end;

    while (it != end)
    {
        string path = fs::relative(it->path(), fs::current_path()).string();

        // Never enter the repository metadata
        if (it->is_directory() &&
            (path == ".mg" || path.rfind(".mg/", 0) == 0))
        {
            it.disable_recursion_pending();
            ++it;
            continue;
        }

        // Don't enter ignored directories
        if (it->is_directory() && ignore.isIgnored(path + "/"))
        {
            it.disable_recursion_pending();
            ++it;
            continue;
        }

        if (it->is_regular_file())
        {
            if (!ignore.isIgnored(path))
            {
                addFile(path);
            }
        }

        ++it;
    }
}