# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "F:\Program Files\Jetbrains\CLion 2021.3.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "F:\Program Files\Jetbrains\CLion 2021.3.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\11096\CLionProjects\sguclient

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\11096\CLionProjects\sguclient\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/sguclient.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sguclient.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sguclient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sguclient.dir/flags.make

CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.obj: CMakeFiles/sguclient.dir/flags.make
CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.obj: CMakeFiles/sguclient.dir/includes_C.rsp
CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.obj: ../sguclient-linux/dprotocol.c
CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.obj: CMakeFiles/sguclient.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\11096\CLionProjects\sguclient\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.obj"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.obj -MF CMakeFiles\sguclient.dir\sguclient-linux\dprotocol.c.obj.d -o CMakeFiles\sguclient.dir\sguclient-linux\dprotocol.c.obj -c C:\Users\11096\CLionProjects\sguclient\sguclient-linux\dprotocol.c

CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.i"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\11096\CLionProjects\sguclient\sguclient-linux\dprotocol.c > CMakeFiles\sguclient.dir\sguclient-linux\dprotocol.c.i

CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.s"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\11096\CLionProjects\sguclient\sguclient-linux\dprotocol.c -o CMakeFiles\sguclient.dir\sguclient-linux\dprotocol.c.s

CMakeFiles/sguclient.dir/sguclient-linux/main.c.obj: CMakeFiles/sguclient.dir/flags.make
CMakeFiles/sguclient.dir/sguclient-linux/main.c.obj: CMakeFiles/sguclient.dir/includes_C.rsp
CMakeFiles/sguclient.dir/sguclient-linux/main.c.obj: ../sguclient-linux/main.c
CMakeFiles/sguclient.dir/sguclient-linux/main.c.obj: CMakeFiles/sguclient.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\11096\CLionProjects\sguclient\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/sguclient.dir/sguclient-linux/main.c.obj"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/sguclient.dir/sguclient-linux/main.c.obj -MF CMakeFiles\sguclient.dir\sguclient-linux\main.c.obj.d -o CMakeFiles\sguclient.dir\sguclient-linux\main.c.obj -c C:\Users\11096\CLionProjects\sguclient\sguclient-linux\main.c

CMakeFiles/sguclient.dir/sguclient-linux/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sguclient.dir/sguclient-linux/main.c.i"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\11096\CLionProjects\sguclient\sguclient-linux\main.c > CMakeFiles\sguclient.dir\sguclient-linux\main.c.i

CMakeFiles/sguclient.dir/sguclient-linux/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sguclient.dir/sguclient-linux/main.c.s"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\11096\CLionProjects\sguclient\sguclient-linux\main.c -o CMakeFiles\sguclient.dir\sguclient-linux\main.c.s

CMakeFiles/sguclient.dir/sguclient-linux/md5.c.obj: CMakeFiles/sguclient.dir/flags.make
CMakeFiles/sguclient.dir/sguclient-linux/md5.c.obj: CMakeFiles/sguclient.dir/includes_C.rsp
CMakeFiles/sguclient.dir/sguclient-linux/md5.c.obj: ../sguclient-linux/md5.c
CMakeFiles/sguclient.dir/sguclient-linux/md5.c.obj: CMakeFiles/sguclient.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\11096\CLionProjects\sguclient\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/sguclient.dir/sguclient-linux/md5.c.obj"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/sguclient.dir/sguclient-linux/md5.c.obj -MF CMakeFiles\sguclient.dir\sguclient-linux\md5.c.obj.d -o CMakeFiles\sguclient.dir\sguclient-linux\md5.c.obj -c C:\Users\11096\CLionProjects\sguclient\sguclient-linux\md5.c

CMakeFiles/sguclient.dir/sguclient-linux/md5.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sguclient.dir/sguclient-linux/md5.c.i"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\11096\CLionProjects\sguclient\sguclient-linux\md5.c > CMakeFiles\sguclient.dir\sguclient-linux\md5.c.i

CMakeFiles/sguclient.dir/sguclient-linux/md5.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sguclient.dir/sguclient-linux/md5.c.s"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\11096\CLionProjects\sguclient\sguclient-linux\md5.c -o CMakeFiles\sguclient.dir\sguclient-linux\md5.c.s

CMakeFiles/sguclient.dir/sguclient-linux/public.c.obj: CMakeFiles/sguclient.dir/flags.make
CMakeFiles/sguclient.dir/sguclient-linux/public.c.obj: CMakeFiles/sguclient.dir/includes_C.rsp
CMakeFiles/sguclient.dir/sguclient-linux/public.c.obj: ../sguclient-linux/public.c
CMakeFiles/sguclient.dir/sguclient-linux/public.c.obj: CMakeFiles/sguclient.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\11096\CLionProjects\sguclient\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/sguclient.dir/sguclient-linux/public.c.obj"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/sguclient.dir/sguclient-linux/public.c.obj -MF CMakeFiles\sguclient.dir\sguclient-linux\public.c.obj.d -o CMakeFiles\sguclient.dir\sguclient-linux\public.c.obj -c C:\Users\11096\CLionProjects\sguclient\sguclient-linux\public.c

CMakeFiles/sguclient.dir/sguclient-linux/public.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sguclient.dir/sguclient-linux/public.c.i"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\11096\CLionProjects\sguclient\sguclient-linux\public.c > CMakeFiles\sguclient.dir\sguclient-linux\public.c.i

CMakeFiles/sguclient.dir/sguclient-linux/public.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sguclient.dir/sguclient-linux/public.c.s"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\11096\CLionProjects\sguclient\sguclient-linux\public.c -o CMakeFiles\sguclient.dir\sguclient-linux\public.c.s

CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.obj: CMakeFiles/sguclient.dir/flags.make
CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.obj: CMakeFiles/sguclient.dir/includes_C.rsp
CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.obj: ../sguclient-linux/sguclient.c
CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.obj: CMakeFiles/sguclient.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\11096\CLionProjects\sguclient\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.obj"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.obj -MF CMakeFiles\sguclient.dir\sguclient-linux\sguclient.c.obj.d -o CMakeFiles\sguclient.dir\sguclient-linux\sguclient.c.obj -c C:\Users\11096\CLionProjects\sguclient\sguclient-linux\sguclient.c

CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.i"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\11096\CLionProjects\sguclient\sguclient-linux\sguclient.c > CMakeFiles\sguclient.dir\sguclient-linux\sguclient.c.i

CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.s"
	"F:\Program Files\Jetbrains\CLion 2021.3.2\bin\mingw\bin\gcc.exe" $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\11096\CLionProjects\sguclient\sguclient-linux\sguclient.c -o CMakeFiles\sguclient.dir\sguclient-linux\sguclient.c.s

# Object files for target sguclient
sguclient_OBJECTS = \
"CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.obj" \
"CMakeFiles/sguclient.dir/sguclient-linux/main.c.obj" \
"CMakeFiles/sguclient.dir/sguclient-linux/md5.c.obj" \
"CMakeFiles/sguclient.dir/sguclient-linux/public.c.obj" \
"CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.obj"

# External object files for target sguclient
sguclient_EXTERNAL_OBJECTS =

sguclient.exe: CMakeFiles/sguclient.dir/sguclient-linux/dprotocol.c.obj
sguclient.exe: CMakeFiles/sguclient.dir/sguclient-linux/main.c.obj
sguclient.exe: CMakeFiles/sguclient.dir/sguclient-linux/md5.c.obj
sguclient.exe: CMakeFiles/sguclient.dir/sguclient-linux/public.c.obj
sguclient.exe: CMakeFiles/sguclient.dir/sguclient-linux/sguclient.c.obj
sguclient.exe: CMakeFiles/sguclient.dir/build.make
sguclient.exe: CMakeFiles/sguclient.dir/linklibs.rsp
sguclient.exe: CMakeFiles/sguclient.dir/objects1.rsp
sguclient.exe: CMakeFiles/sguclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\11096\CLionProjects\sguclient\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable sguclient.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\sguclient.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sguclient.dir/build: sguclient.exe
.PHONY : CMakeFiles/sguclient.dir/build

CMakeFiles/sguclient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\sguclient.dir\cmake_clean.cmake
.PHONY : CMakeFiles/sguclient.dir/clean

CMakeFiles/sguclient.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\11096\CLionProjects\sguclient C:\Users\11096\CLionProjects\sguclient C:\Users\11096\CLionProjects\sguclient\cmake-build-debug C:\Users\11096\CLionProjects\sguclient\cmake-build-debug C:\Users\11096\CLionProjects\sguclient\cmake-build-debug\CMakeFiles\sguclient.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sguclient.dir/depend
