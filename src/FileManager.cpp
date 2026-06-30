#include "../include/FileManager.h"

#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

bool FileManager::exists(const string &filename)
{
    return fs::exists(filename);
}

bool FileManager::copyToStage(const string &filename)
{
    try
    {
        fs::path destination = fs::path(".mg/staging") / filename;

        // Create parent directories if needed
        fs::create_directories(destination.parent_path());

        fs::copy_file(
            filename,
            destination,
            fs::copy_options::overwrite_existing
        );

        return true;
    }
    catch (const fs::filesystem_error &)
    {
        return false;
    }
}