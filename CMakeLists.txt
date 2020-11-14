cmake_minimum_required(VERSION 3.17)
project(lit)

set(CMAKE_CXX_STANDARD 17)

# Structs
add_library(litStructs Lit/LitStructs.cpp)
target_include_directories(litStructs PUBLIC Lit)
target_include_directories(litStructs PUBLIC Lit)

# Helpers
add_library(constants Helpers/Constants.cpp)
add_library(fileSystemHelper Helpers/FileSystemHelper.cpp)
target_link_libraries(fileSystemHelper PUBLIC constants)
add_library(shellCommands Helpers/ShellCommands.cpp)
target_link_libraries(shellCommands PUBLIC fileSystemHelper)
add_library(litCommandsHelper Helpers/LitCommandsHelper.cpp)
target_link_libraries(litCommandsHelper PUBLIC constants fileSystemHelper litStructs shellCommands)
target_include_directories(litCommandsHelper PUBLIC Helpers)

# Lit
add_library(litCommands Lit/LitCommands.cpp Lit/LitStructs.cpp)
target_link_libraries(litCommands PUBLIC shellCommands fileSystemHelper litCommandsHelper)

# Lit
add_executable(lit lit.cpp)
target_link_libraries(lit PRIVATE litStructs litCommands)
target_compile_options(lit PRIVATE -Wall -Wextra -pedantic -Werror)
