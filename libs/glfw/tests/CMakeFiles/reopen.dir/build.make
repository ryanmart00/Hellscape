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
include libs/glfw/tests/CMakeFiles/reopen.dir/depend.make

# Include the progress variables for this target.
include libs/glfw/tests/CMakeFiles/reopen.dir/progress.make

# Include the compile flags for this target's objects.
include libs/glfw/tests/CMakeFiles/reopen.dir/flags.make

libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o: libs/glfw/tests/CMakeFiles/reopen.dir/flags.make
libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o: libs/glfw/tests/reopen.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ryan/Documents/Game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/reopen.dir/reopen.c.o   -c /home/ryan/Documents/Game/libs/glfw/tests/reopen.c

libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/reopen.dir/reopen.c.i"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ryan/Documents/Game/libs/glfw/tests/reopen.c > CMakeFiles/reopen.dir/reopen.c.i

libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/reopen.dir/reopen.c.s"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ryan/Documents/Game/libs/glfw/tests/reopen.c -o CMakeFiles/reopen.dir/reopen.c.s

libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o.requires:

.PHONY : libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o.requires

libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o.provides: libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o.requires
	$(MAKE) -f libs/glfw/tests/CMakeFiles/reopen.dir/build.make libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o.provides.build
.PHONY : libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o.provides

libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o.provides.build: libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o


libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o: libs/glfw/tests/CMakeFiles/reopen.dir/flags.make
libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o: libs/glfw/deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ryan/Documents/Game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/reopen.dir/__/deps/glad_gl.c.o   -c /home/ryan/Documents/Game/libs/glfw/deps/glad_gl.c

libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/reopen.dir/__/deps/glad_gl.c.i"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ryan/Documents/Game/libs/glfw/deps/glad_gl.c > CMakeFiles/reopen.dir/__/deps/glad_gl.c.i

libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/reopen.dir/__/deps/glad_gl.c.s"
	cd /home/ryan/Documents/Game/libs/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ryan/Documents/Game/libs/glfw/deps/glad_gl.c -o CMakeFiles/reopen.dir/__/deps/glad_gl.c.s

libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o.requires:

.PHONY : libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o.requires

libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o.provides: libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o.requires
	$(MAKE) -f libs/glfw/tests/CMakeFiles/reopen.dir/build.make libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o.provides.build
.PHONY : libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o.provides

libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o.provides.build: libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o


# Object files for target reopen
reopen_OBJECTS = \
"CMakeFiles/reopen.dir/reopen.c.o" \
"CMakeFiles/reopen.dir/__/deps/glad_gl.c.o"

# External object files for target reopen
reopen_EXTERNAL_OBJECTS =

libs/glfw/tests/reopen: libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o
libs/glfw/tests/reopen: libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o
libs/glfw/tests/reopen: libs/glfw/tests/CMakeFiles/reopen.dir/build.make
libs/glfw/tests/reopen: libs/glfw/src/libglfw3.a
libs/glfw/tests/reopen: /usr/lib/x86_64-linux-gnu/libm.so
libs/glfw/tests/reopen: /usr/lib/x86_64-linux-gnu/librt.so
libs/glfw/tests/reopen: /usr/lib/x86_64-linux-gnu/libm.so
libs/glfw/tests/reopen: /usr/lib/x86_64-linux-gnu/libX11.so
libs/glfw/tests/reopen: libs/glfw/tests/CMakeFiles/reopen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ryan/Documents/Game/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable reopen"
	cd /home/ryan/Documents/Game/libs/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/reopen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/glfw/tests/CMakeFiles/reopen.dir/build: libs/glfw/tests/reopen

.PHONY : libs/glfw/tests/CMakeFiles/reopen.dir/build

libs/glfw/tests/CMakeFiles/reopen.dir/requires: libs/glfw/tests/CMakeFiles/reopen.dir/reopen.c.o.requires
libs/glfw/tests/CMakeFiles/reopen.dir/requires: libs/glfw/tests/CMakeFiles/reopen.dir/__/deps/glad_gl.c.o.requires

.PHONY : libs/glfw/tests/CMakeFiles/reopen.dir/requires

libs/glfw/tests/CMakeFiles/reopen.dir/clean:
	cd /home/ryan/Documents/Game/libs/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/reopen.dir/cmake_clean.cmake
.PHONY : libs/glfw/tests/CMakeFiles/reopen.dir/clean

libs/glfw/tests/CMakeFiles/reopen.dir/depend:
	cd /home/ryan/Documents/Game && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ryan/Documents/Game /home/ryan/Documents/Game/libs/glfw/tests /home/ryan/Documents/Game /home/ryan/Documents/Game/libs/glfw/tests /home/ryan/Documents/Game/libs/glfw/tests/CMakeFiles/reopen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/glfw/tests/CMakeFiles/reopen.dir/depend

