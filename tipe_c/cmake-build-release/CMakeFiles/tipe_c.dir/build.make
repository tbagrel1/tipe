# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /home/thomas/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/173.3727.114/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/thomas/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/173.3727.114/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/tipe_c.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tipe_c.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tipe_c.dir/flags.make

CMakeFiles/tipe_c.dir/main.c.o: CMakeFiles/tipe_c.dir/flags.make
CMakeFiles/tipe_c.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tipe_c.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tipe_c.dir/main.c.o   -c /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/main.c

CMakeFiles/tipe_c.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tipe_c.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/main.c > CMakeFiles/tipe_c.dir/main.c.i

CMakeFiles/tipe_c.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tipe_c.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/main.c -o CMakeFiles/tipe_c.dir/main.c.s

CMakeFiles/tipe_c.dir/main.c.o.requires:

.PHONY : CMakeFiles/tipe_c.dir/main.c.o.requires

CMakeFiles/tipe_c.dir/main.c.o.provides: CMakeFiles/tipe_c.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/tipe_c.dir/build.make CMakeFiles/tipe_c.dir/main.c.o.provides.build
.PHONY : CMakeFiles/tipe_c.dir/main.c.o.provides

CMakeFiles/tipe_c.dir/main.c.o.provides.build: CMakeFiles/tipe_c.dir/main.c.o


CMakeFiles/tipe_c.dir/bwt_2.c.o: CMakeFiles/tipe_c.dir/flags.make
CMakeFiles/tipe_c.dir/bwt_2.c.o: ../bwt_2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/tipe_c.dir/bwt_2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tipe_c.dir/bwt_2.c.o   -c /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/bwt_2.c

CMakeFiles/tipe_c.dir/bwt_2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tipe_c.dir/bwt_2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/bwt_2.c > CMakeFiles/tipe_c.dir/bwt_2.c.i

CMakeFiles/tipe_c.dir/bwt_2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tipe_c.dir/bwt_2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/bwt_2.c -o CMakeFiles/tipe_c.dir/bwt_2.c.s

CMakeFiles/tipe_c.dir/bwt_2.c.o.requires:

.PHONY : CMakeFiles/tipe_c.dir/bwt_2.c.o.requires

CMakeFiles/tipe_c.dir/bwt_2.c.o.provides: CMakeFiles/tipe_c.dir/bwt_2.c.o.requires
	$(MAKE) -f CMakeFiles/tipe_c.dir/build.make CMakeFiles/tipe_c.dir/bwt_2.c.o.provides.build
.PHONY : CMakeFiles/tipe_c.dir/bwt_2.c.o.provides

CMakeFiles/tipe_c.dir/bwt_2.c.o.provides.build: CMakeFiles/tipe_c.dir/bwt_2.c.o


CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o: CMakeFiles/tipe_c.dir/flags.make
CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o: ../bwt_2_launcher.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o   -c /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/bwt_2_launcher.c

CMakeFiles/tipe_c.dir/bwt_2_launcher.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tipe_c.dir/bwt_2_launcher.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/bwt_2_launcher.c > CMakeFiles/tipe_c.dir/bwt_2_launcher.c.i

CMakeFiles/tipe_c.dir/bwt_2_launcher.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tipe_c.dir/bwt_2_launcher.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/bwt_2_launcher.c -o CMakeFiles/tipe_c.dir/bwt_2_launcher.c.s

CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o.requires:

.PHONY : CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o.requires

CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o.provides: CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o.requires
	$(MAKE) -f CMakeFiles/tipe_c.dir/build.make CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o.provides.build
.PHONY : CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o.provides

CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o.provides.build: CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o


CMakeFiles/tipe_c.dir/suffix_array/clist.c.o: CMakeFiles/tipe_c.dir/flags.make
CMakeFiles/tipe_c.dir/suffix_array/clist.c.o: ../suffix_array/clist.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/tipe_c.dir/suffix_array/clist.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tipe_c.dir/suffix_array/clist.c.o   -c /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/suffix_array/clist.c

CMakeFiles/tipe_c.dir/suffix_array/clist.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tipe_c.dir/suffix_array/clist.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/suffix_array/clist.c > CMakeFiles/tipe_c.dir/suffix_array/clist.c.i

CMakeFiles/tipe_c.dir/suffix_array/clist.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tipe_c.dir/suffix_array/clist.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/suffix_array/clist.c -o CMakeFiles/tipe_c.dir/suffix_array/clist.c.s

CMakeFiles/tipe_c.dir/suffix_array/clist.c.o.requires:

.PHONY : CMakeFiles/tipe_c.dir/suffix_array/clist.c.o.requires

CMakeFiles/tipe_c.dir/suffix_array/clist.c.o.provides: CMakeFiles/tipe_c.dir/suffix_array/clist.c.o.requires
	$(MAKE) -f CMakeFiles/tipe_c.dir/build.make CMakeFiles/tipe_c.dir/suffix_array/clist.c.o.provides.build
.PHONY : CMakeFiles/tipe_c.dir/suffix_array/clist.c.o.provides

CMakeFiles/tipe_c.dir/suffix_array/clist.c.o.provides.build: CMakeFiles/tipe_c.dir/suffix_array/clist.c.o


CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o: CMakeFiles/tipe_c.dir/flags.make
CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o: ../suffix_array/suffix_array.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o   -c /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/suffix_array/suffix_array.c

CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/suffix_array/suffix_array.c > CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.i

CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/suffix_array/suffix_array.c -o CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.s

CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o.requires:

.PHONY : CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o.requires

CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o.provides: CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o.requires
	$(MAKE) -f CMakeFiles/tipe_c.dir/build.make CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o.provides.build
.PHONY : CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o.provides

CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o.provides.build: CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o


# Object files for target tipe_c
tipe_c_OBJECTS = \
"CMakeFiles/tipe_c.dir/main.c.o" \
"CMakeFiles/tipe_c.dir/bwt_2.c.o" \
"CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o" \
"CMakeFiles/tipe_c.dir/suffix_array/clist.c.o" \
"CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o"

# External object files for target tipe_c
tipe_c_EXTERNAL_OBJECTS =

tipe_c: CMakeFiles/tipe_c.dir/main.c.o
tipe_c: CMakeFiles/tipe_c.dir/bwt_2.c.o
tipe_c: CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o
tipe_c: CMakeFiles/tipe_c.dir/suffix_array/clist.c.o
tipe_c: CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o
tipe_c: CMakeFiles/tipe_c.dir/build.make
tipe_c: CMakeFiles/tipe_c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable tipe_c"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tipe_c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tipe_c.dir/build: tipe_c

.PHONY : CMakeFiles/tipe_c.dir/build

CMakeFiles/tipe_c.dir/requires: CMakeFiles/tipe_c.dir/main.c.o.requires
CMakeFiles/tipe_c.dir/requires: CMakeFiles/tipe_c.dir/bwt_2.c.o.requires
CMakeFiles/tipe_c.dir/requires: CMakeFiles/tipe_c.dir/bwt_2_launcher.c.o.requires
CMakeFiles/tipe_c.dir/requires: CMakeFiles/tipe_c.dir/suffix_array/clist.c.o.requires
CMakeFiles/tipe_c.dir/requires: CMakeFiles/tipe_c.dir/suffix_array/suffix_array.c.o.requires

.PHONY : CMakeFiles/tipe_c.dir/requires

CMakeFiles/tipe_c.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tipe_c.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tipe_c.dir/clean

CMakeFiles/tipe_c.dir/depend:
	cd /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release /home/thomas/Drive/tbagrel/courses/courses_2017_2018/tipe/tipe_c/cmake-build-release/CMakeFiles/tipe_c.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tipe_c.dir/depend

