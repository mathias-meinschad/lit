# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/129/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/129/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mottl/workspace/lit

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mottl/workspace/lit/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/litCommandsHelper.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/litCommandsHelper.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/litCommandsHelper.dir/flags.make

CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.o: CMakeFiles/litCommandsHelper.dir/flags.make
CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.o: ../Helpers/LitCommandsHelper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mottl/workspace/lit/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.o -c /home/mottl/workspace/lit/Helpers/LitCommandsHelper.cpp

CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mottl/workspace/lit/Helpers/LitCommandsHelper.cpp > CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.i

CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mottl/workspace/lit/Helpers/LitCommandsHelper.cpp -o CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.s

# Object files for target litCommandsHelper
litCommandsHelper_OBJECTS = \
"CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.o"

# External object files for target litCommandsHelper
litCommandsHelper_EXTERNAL_OBJECTS =

liblitCommandsHelper.a: CMakeFiles/litCommandsHelper.dir/Helpers/LitCommandsHelper.cpp.o
liblitCommandsHelper.a: CMakeFiles/litCommandsHelper.dir/build.make
liblitCommandsHelper.a: CMakeFiles/litCommandsHelper.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mottl/workspace/lit/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblitCommandsHelper.a"
	$(CMAKE_COMMAND) -P CMakeFiles/litCommandsHelper.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/litCommandsHelper.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/litCommandsHelper.dir/build: liblitCommandsHelper.a

.PHONY : CMakeFiles/litCommandsHelper.dir/build

CMakeFiles/litCommandsHelper.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/litCommandsHelper.dir/cmake_clean.cmake
.PHONY : CMakeFiles/litCommandsHelper.dir/clean

CMakeFiles/litCommandsHelper.dir/depend:
	cd /home/mottl/workspace/lit/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mottl/workspace/lit /home/mottl/workspace/lit /home/mottl/workspace/lit/cmake-build-debug /home/mottl/workspace/lit/cmake-build-debug /home/mottl/workspace/lit/cmake-build-debug/CMakeFiles/litCommandsHelper.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/litCommandsHelper.dir/depend
