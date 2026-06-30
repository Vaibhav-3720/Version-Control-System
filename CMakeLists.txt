cmake_minimum_required(VERSION 3.16)

project(MiniGit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(include)
add_executable(
    mg
    src/main.cpp
    src/CLI.cpp
    src/Repository.cpp
    src/FileManager.cpp
    src/Index.cpp
    src/Commit.cpp
    src/Logger.cpp
    src/Checkout.cpp
    src/Status.cpp
    src/Diff.cpp
    src/Ignore.cpp
    src/Graph.cpp
    src/Utils.cpp
)