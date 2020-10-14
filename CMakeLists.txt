cmake_minimum_required(VERSION 3.17)
project(lit)

set(CMAKE_CXX_STANDARD 17)

add_executable(lit main.cpp Tree/Tree.cpp Tree/Tree.h Commit/Commit.cpp Commit/Commit.h Blob/Blob.cpp Blob/Blob.h Lit/LitCommands.cpp Lit/LitCommands.h)
