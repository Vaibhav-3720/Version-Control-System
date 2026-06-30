#include <iostream>
#include <vector>
#include <string>

#include "../include/CLI.h"

using namespace std;

int main(int argc, char* argv[])
{
    vector<string> args;

    // Store all command-line arguments except the executable name
    for(int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    CLI cli;
    cli.run(args);

    return 0;
}