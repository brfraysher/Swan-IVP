# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI

# Include any dependencies generated for this target.
include Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/depend.make

# Include the progress variables for this target.
include Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/progress.make

# Include the compile flags for this target's objects.
include Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/flags.make

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/flags.make
Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o: /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/MOOSRemoteLite.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o"
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o -c /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/MOOSRemoteLite.cpp

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.i"
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/MOOSRemoteLite.cpp > CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.i

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.s"
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/MOOSRemoteLite.cpp -o CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.s

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o.requires:

.PHONY : Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o.requires

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o.provides: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o.requires
	$(MAKE) -f Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/build.make Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o.provides.build
.PHONY : Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o.provides

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o.provides.build: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o


Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/flags.make
Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o: /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/iRemoteLiteMain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o"
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o -c /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/iRemoteLiteMain.cpp

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.i"
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/iRemoteLiteMain.cpp > CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.i

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.s"
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/iRemoteLiteMain.cpp -o CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.s

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o.requires:

.PHONY : Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o.requires

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o.provides: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o.requires
	$(MAKE) -f Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/build.make Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o.provides.build
.PHONY : Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o.provides

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o.provides.build: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o


# Object files for target iRemoteLite
iRemoteLite_OBJECTS = \
"CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o" \
"CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o"

# External object files for target iRemoteLite
iRemoteLite_EXTERNAL_OBJECTS =

/home/ansucato/Song/kayak/moos-ivp/bin/iRemoteLite: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o
/home/ansucato/Song/kayak/moos-ivp/bin/iRemoteLite: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o
/home/ansucato/Song/kayak/moos-ivp/bin/iRemoteLite: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/build.make
/home/ansucato/Song/kayak/moos-ivp/bin/iRemoteLite: /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSCore/lib/libMOOS.a
/home/ansucato/Song/kayak/moos-ivp/bin/iRemoteLite: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable /home/ansucato/Song/kayak/moos-ivp/bin/iRemoteLite"
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/iRemoteLite.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/build: /home/ansucato/Song/kayak/moos-ivp/bin/iRemoteLite

.PHONY : Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/build

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/requires: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/MOOSRemoteLite.cpp.o.requires
Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/requires: Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/iRemoteLiteMain.cpp.o.requires

.PHONY : Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/requires

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/clean:
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite && $(CMAKE_COMMAND) -P CMakeFiles/iRemoteLite.dir/cmake_clean.cmake
.PHONY : Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/clean

Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/depend:
	cd /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI /home/ansucato/Song/kayak/moos-ivp/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite /home/ansucato/Song/kayak/moos-ivp/build/MOOS/MOOSToolsUI/Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Tools/Console/iRemoteLite/CMakeFiles/iRemoteLite.dir/depend

