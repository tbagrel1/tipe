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


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /home/thomas/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/181.4445.84/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/thomas/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/181.4445.84/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug

# Include any dependencies generated for this target.
include examples/CMakeFiles/suftest.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/suftest.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/suftest.dir/flags.make

examples/CMakeFiles/suftest.dir/suftest.o: examples/CMakeFiles/suftest.dir/flags.make
examples/CMakeFiles/suftest.dir/suftest.o: ../examples/suftest.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/CMakeFiles/suftest.dir/suftest.o"
	cd /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/suftest.dir/suftest.o   -c /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/examples/suftest.c

examples/CMakeFiles/suftest.dir/suftest.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/suftest.dir/suftest.i"
	cd /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/examples/suftest.c > CMakeFiles/suftest.dir/suftest.i

examples/CMakeFiles/suftest.dir/suftest.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/suftest.dir/suftest.s"
	cd /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/examples/suftest.c -o CMakeFiles/suftest.dir/suftest.s

examples/CMakeFiles/suftest.dir/suftest.o.requires:

.PHONY : examples/CMakeFiles/suftest.dir/suftest.o.requires

examples/CMakeFiles/suftest.dir/suftest.o.provides: examples/CMakeFiles/suftest.dir/suftest.o.requires
	$(MAKE) -f examples/CMakeFiles/suftest.dir/build.make examples/CMakeFiles/suftest.dir/suftest.o.provides.build
.PHONY : examples/CMakeFiles/suftest.dir/suftest.o.provides

examples/CMakeFiles/suftest.dir/suftest.o.provides.build: examples/CMakeFiles/suftest.dir/suftest.o


# Object files for target suftest
suftest_OBJECTS = \
"CMakeFiles/suftest.dir/suftest.o"

# External object files for target suftest
suftest_EXTERNAL_OBJECTS =

examples/suftest: examples/CMakeFiles/suftest.dir/suftest.o
examples/suftest: examples/CMakeFiles/suftest.dir/build.make
examples/suftest: lib/libsais.a
examples/suftest: examples/CMakeFiles/suftest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable suftest"
	cd /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/suftest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/suftest.dir/build: examples/suftest

.PHONY : examples/CMakeFiles/suftest.dir/build

examples/CMakeFiles/suftest.dir/requires: examples/CMakeFiles/suftest.dir/suftest.o.requires

.PHONY : examples/CMakeFiles/suftest.dir/requires

examples/CMakeFiles/suftest.dir/clean:
	cd /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/examples && $(CMAKE_COMMAND) -P CMakeFiles/suftest.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/suftest.dir/clean

examples/CMakeFiles/suftest.dir/depend:
	cd /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1 /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/examples /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/examples /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/resources/sais-2.4.1/cmake-build-debug/examples/CMakeFiles/suftest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/suftest.dir/depend
