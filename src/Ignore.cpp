#include "../include/Ignore.h"

#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

Ignore::Ignore()
{
    ifstream file(".mgignore");

    if (!file)
        return;

    string pattern;

    while (getline(file, pattern))
    {
        if (pattern.empty())
            continue;

        while (!pattern.empty() &&
               (pattern.back() == '\r' ||
                pattern.back() == '\n'))
        {
            pattern.pop_back();
        }

        patterns.push_back(pattern);
    }
}

bool Ignore::isIgnored(const string &path)
{
    for (const string &pattern : patterns)
    {
        // Folder
        if (!pattern.empty() && pattern.back() == '/')
        {
            string folder = pattern.substr(0, pattern.size() - 1);

            if (path == folder ||
                path.rfind(folder + "/", 0) == 0)
            {
                return true;
            }
        }

        // *.extension
        else if (pattern.size() > 2 &&
                 pattern[0] == '*' &&
                 pattern[1] == '.')
        {
            string ext = pattern.substr(1);

            if (fs::path(path).extension() == ext)
                return true;
        }

        // Exact filename
        else
        {
            if (fs::path(path).filename() == pattern)
                return true;
        }
    }

    return false;
}