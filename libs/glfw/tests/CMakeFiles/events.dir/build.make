# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/ryan/Documents/Game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ryan/Documents/Game

# Include any dependencies generated for this target.
include libs/glfw/tests/CMakeFiles/events.dir/depend.make

# Include the progress variables for this target.
include libs/glfw/tests/CMakeFiles/events.dir/progress.make

# Include the compile flags for this target's objects.
include libs/glfw/tests/CMakeFiles/events.dir/flags.make

libs/glfw/tests/CMakeFiles/events.dir/events.c.o: libs/glfw/tests/CMakeFiles/events.dir/flags.make
libs/glfw/tests/CMakeFiles/events.dir/events.c.o: libs/glfw/tests/events.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ryan/Documents/Game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object libs/glfw/tests/CMakeFiles/events.dir/events.c.o"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/events.dir/events.c.o   -c /home/ryan/Documents/Game/libs/glfw/tests/events.c

libs/glfw/tests/CMakeFiles/events.dir/events.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/events.dir/events.c.i"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ryan/Documents/Game/libs/glfw/tests/events.c > CMakeFiles/events.dir/events.c.i

libs/glfw/tests/CMakeFiles/events.dir/events.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/events.dir/events.c.s"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ryan/Documents/Game/libs/glfw/tests/events.c -o CMakeFiles/events.dir/events.c.s

libs/glfw/tests/CMakeFiles/events.dir/events.c.o.requires:

.PHONY : libs/glfw/tests/CMakeFiles/events.dir/events.c.o.requires

libs/glfw/tests/CMakeFiles/events.dir/events.c.o.provides: libs/glfw/tests/CMakeFiles/events.dir/events.c.o.requires
	$(MAKE) -f libs/glfw/tests/CMakeFiles/events.dir/build.make libs/glfw/tests/CMakeFiles/events.dir/events.c.o.provides.build
.PHONY : libs/glfw/tests/CMakeFiles/events.dir/events.c.o.provides

libs/glfw/tests/CMakeFiles/events.dir/events.c.o.provides.build: libs/glfw/tests/CMakeFiles/events.dir/events.c.o


libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o: libs/glfw/tests/CMakeFiles/events.dir/flags.make
libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o: libs/glfw/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ryan/Documents/Game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/events.dir/__/deps/getopt.c.o   -c /home/ryan/Documents/Game/libs/glfw/deps/getopt.c

libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/events.dir/__/deps/getopt.c.i"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ryan/Documents/Game/libs/glfw/deps/getopt.c > CMakeFiles/events.dir/__/deps/getopt.c.i

libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/events.dir/__/deps/getopt.c.s"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ryan/Documents/Game/libs/glfw/deps/getopt.c -o CMakeFiles/events.dir/__/deps/getopt.c.s

libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o.requires:

.PHONY : libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o.requires

libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o.provides: libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o.requires
	$(MAKE) -f libs/glfw/tests/CMakeFiles/events.dir/build.make libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o.provides.build
.PHONY : libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o.provides

libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o.provides.build: libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o


libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o: libs/glfw/tests/CMakeFiles/events.dir/flags.make
libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o: libs/glfw/deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ryan/Documents/Game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/events.dir/__/deps/glad_gl.c.o   -c /home/ryan/Documents/Game/libs/glfw/deps/glad_gl.c

libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/events.dir/__/deps/glad_gl.c.i"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ryan/Documents/Game/libs/glfw/deps/glad_gl.c > CMakeFiles/events.dir/__/deps/glad_gl.c.i

libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/events.dir/__/deps/glad_gl.c.s"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ryan/Documents/Game/libs/glfw/deps/glad_gl.c -o CMakeFiles/events.dir/__/deps/glad_gl.c.s

libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o.requires:

.PHONY : libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o.requires

libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o.provides: libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o.requires
	$(MAKE) -f libs/glfw/tests/CMakeFiles/events.dir/build.make libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o.provides.build
.PHONY : libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o.provides

libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o.provides.build: libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o


# Object files for target events
events_OBJECTS = \
"CMakeFiles/events.dir/events.c.o" \
"CMakeFiles/events.dir/__/deps/getopt.c.o" \
"CMakeFiles/events.dir/__/deps/glad_gl.c.o"

# External object files for target events
events_EXTERNAL_OBJECTS =

libs/glfw/tests/events: libs/glfw/tests/CMakeFiles/events.dir/events.c.o
libs/glfw/tests/events: libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o
libs/glfw/tests/events: libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o
libs/glfw/tests/events: libs/glfw/tests/CMakeFiles/events.dir/build.make
libs/glfw/tests/events: libs/glfw/src/libglfw3.a
libs/glfw/tests/events: /usr/lib/x86_64-linux-gnu/libm.so
libs/glfw/tests/events: /usr/lib/x86_64-linux-gnu/librt.so
libs/glfw/tests/events: /usr/lib/x86_64-linux-gnu/libm.so
libs/glfw/tests/events: /usr/lib/x86_64-linux-gnu/libX11.so
libs/glfw/tests/events: libs/glfw/tests/CMakeFiles/events.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ryan/Documents/Game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable events"
	cd /home/ryan/Documents/Game/libs/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/events.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/glfw/tests/CMakeFiles/events.dir/build: libs/glfw/tests/events

.PHONY : libs/glfw/tests/CMakeFiles/events.dir/build

libs/glfw/tests/CMakeFiles/events.dir/requires: libs/glfw/tests/CMakeFiles/events.dir/events.c.o.requires
libs/glfw/tests/CMakeFiles/events.dir/requires: libs/glfw/tests/CMakeFiles/events.dir/__/deps/getopt.c.o.requires
libs/glfw/tests/CMakeFiles/events.dir/requires: libs/glfw/tests/CMakeFiles/events.dir/__/deps/glad_gl.c.o.requires

.PHONY : libs/glfw/tests/CMakeFiles/events.dir/requires

libs/glfw/tests/CMakeFiles/events.dir/clean:
	cd /home/ryan/Documents/Game/libs/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/events.dir/cmake_clean.cmake
.PHONY : libs/glfw/tests/CMakeFiles/events.dir/clean

libs/glfw/tests/CMakeFiles/events.dir/depend:
	cd /home/ryan/Documents/Game && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ryan/Documents/Game /home/ryan/Documents/Game/libs/glfw/tests /home/ryan/Documents/Game /home/ryan/Documents/Game/libs/glfw/tests /home/ryan/Documents/Game/libs/glfw/tests/CMakeFiles/events.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/glfw/tests/CMakeFiles/events.dir/depend

