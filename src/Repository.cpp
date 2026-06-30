#include "../include/Repository.h"
#include "../include/Index.h"
#include "../include/Commit.h"
#include "../include/Logger.h"
#include "../include/Checkout.h"
#include "../include/Status.h"
#include "../include/Diff.h"
#include "../include/Graph.h"

#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

// init()
void Repository::init()
{
    if (fs::exists(".mg"))
    {
        cout << "Repository already exists.\n";
        return;
    }

    fs::create_directory(".mg");
    fs::create_directory(".mg/commits");
    fs::create_directory(".mg/staging");

    ofstream head(".mg/HEAD");
    head << 0;
    head.close();
    ofstream index(".mg/index");
    ofstream(".mg/config");

    cout << "Initialized empty MiniGit repository.\n";
}

// add()
void Repository::add(const string &path)
{
    Index index;

    if (path == ".")
    {
        // We'll implement this next
        index.addAllFiles();
    }
    else
    {
        index.addFile(path);
    }
}
void Repository::commit(const string &message)
{
    Commit commit;

    commit.create(message);
}
void Repository::log()
{
    Logger logger;

    logger.printLog();
}
void Repository::checkout(int commitID)
{
    Checkout checkout;

    checkout.restore(commitID);
}
void Repository::status()
{
    Status s;
    s.print();
}
void Repository::reset()
{
    Index index;

    index.clear();
}
void Repository::rm(const string &filename)
{
    Index index;

    index.removeFile(filename);
}
void Repository::diff()
{
    Diff diff;
    diff.show();
}
void Repository::graph()
{
    Graph graph;
    graph.show();
}