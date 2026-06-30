#include "../include/CLI.h"
#include "../include/Repository.h"

#include <iostream>

using namespace std;

void CLI::run(const vector<string> &args)
{
    if (args.empty())
    {
        cout << "MiniGit v1.0\n\n";

        cout << "Available Commands\n";
        cout << "------------------\n";

        cout << "mg init\n";
        cout << "mg add <file>\n";
        cout << "mg add .\n";
        cout << "mg commit -m \"message\"\n";
        cout << "mg log\n";
        cout << "mg status\n";
        cout << "mg diff\n";
        cout << "mg checkout <commit-id>\n";
        cout << "mg reset\n";
        cout << "mg rm <file>\n";
        cout << "mg help\n";

        return;
    }
    string command = args[0];

    if (command == "init")
    {
        Repository repo;

        repo.init();
    }
    else if (command == "add")
    {
        if (args.size() < 2)
        {
            cout << "Please provide a file name.\n";
            return;
        }

        Repository repo;

        repo.add(args[1]);
    }
    else if (command == "commit")
    {
        if (args.size() < 3)
        {
            cout << "Usage: mg commit -m \"message\"\n";
            return;
        }

        Repository repo;
        repo.commit(args[2]);
    }
    else if (command == "log")
    {
        Repository repo;
        repo.log();
    }
    else if (command == "checkout")
    {
        if (args.size() != 2)
        {
            cout << "Usage: mg checkout <commit-id>\n";
            return;
        }

        Repository repo;

        try
        {
            repo.checkout(stoi(args[1]));
        }
        catch (...)
        {
            cout << "Invalid commit ID.\n";
        }
    }
    else if (command == "status")
    {
        Repository repo;
        repo.status();
    }
    else if (command == "reset")
    {
        Repository repo;
        repo.reset();
    }
    else if (command == "rm")
    {
        if (args.size() != 2)
        {
            cout << "Usage: mg rm <file>\n";
            return;
        }

        Repository repo;
        repo.rm(args[1]);
    }
    else if (command == "diff")
    {
        Repository repo;
        repo.diff();
    }
    else if (command == "help")
    {
        cout << "MiniGit v1.0\n\n";

        cout << "Available Commands\n";
        cout << "------------------\n";

        cout << "init                 Initialize a repository\n";
        cout << "add <file>           Stage a file\n";
        cout << "add .                Stage all files\n";
        cout << "commit -m <msg>      Create a commit\n";
        cout << "status               Show repository status\n";
        cout << "log                  Show commit history\n";
        cout << "diff                 Show uncommitted changes\n";
        cout << "checkout <id>        Restore a commit\n";
        cout << "reset                Clear staging area\n";
        cout << "rm <file>            Unstage a file\n";
    }
    else if (command == "graph")
    {
        Repository repo;
        repo.graph();
    }
    else
    {
        cout << "Unknown command.\n";
    }
}