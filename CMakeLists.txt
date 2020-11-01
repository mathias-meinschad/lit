cmake_minimum_required(VERSION 3.17)
project(lit)

set(CMAKE_CXX_STANDARD 17)

add_library(listStructs Lit/LitStructs.cpp)
target_include_directories(listStructs PUBLIC Lit)

add_library(commit Commit/Commit.cpp)
target_include_directories(commit PUBLIC Commit)

add_library(fileSystemHelper Helpers/FileSystemHelper.cpp)
add_library(constants Helpers/Constants.cpp)
add_library(shellCommands Helpers/ShellCommands.cpp)
target_include_directories(shellCommands PUBLIC Helpers)

add_library(litCommands Lit/LitCommands.cpp Lit/LitStructs.cpp)
target_include_directories(litCommands PUBLIC Lit)
target_link_libraries(litCommands PUBLIC shellCommands commit fileSystemHelper)

add_executable(lit lit.cpp)
target_link_libraries(lit PRIVATE commit litCommands shellCommands fileSystemHelper constants)
target_compile_options(lit PRIVATE -Wall -Wextra -pedantic -Werror)
