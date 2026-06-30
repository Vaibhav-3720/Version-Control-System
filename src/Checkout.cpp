#include "../include/Checkout.h"
#include "../include/Utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

void Checkout::restore(int commitID)
{
    try
    {
        // Check if repository exists
        if (!fs::exists(".mg"))
        {
            cout << "Not a MiniGit repository.\n";
            return;
        }

        // Check if commit exists
        string commitFolder = ".mg/commits/" + to_string(commitID);

        if (!fs::exists(commitFolder))
        {
            cout << "Commit " << commitID << " does not exist.\n";
            return;
        }

        // Check if snapshot exists
        string snapshotFolder = commitFolder + "/snapshot";

        if (!fs::exists(snapshotFolder))
        {
            cout << "Snapshot is missing.\n";
            return;
        }

        // Delete current working directory contents
        for (const auto &entry : fs::directory_iterator("."))
        {
            string name = entry.path().filename().string();

            // Never delete repository metadata
            if (name == ".mg")
                continue;

            fs::remove_all(entry.path());
        }

        // Restore snapshot
        for (const auto &entry : fs::directory_iterator(snapshotFolder))
        {
            fs::copy(
                entry.path(),
                fs::current_path() / entry.path().filename(),
                fs::copy_options::recursive |
                    fs::copy_options::overwrite_existing);
        }

        // Update HEAD
        ofstream head(".mg/HEAD");

        if (!head)
        {
            cout << "Failed to update HEAD.\n";
            return;
        }

        head << commitID;
        head.close();

        cout << "Checked out "
             << Utils::getCommitHash(commitID)
             << " successfully.\n";
    }
    catch (const fs::filesystem_error &e)
    {
        cout << "Filesystem Error: " << e.what() << endl;
    }
}