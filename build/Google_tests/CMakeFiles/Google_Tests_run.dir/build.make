# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/tools/cmake3.19-gnu/bin/cmake

# The command to remove a file.
RM = /opt/tools/cmake3.19-gnu/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /stck/aremigi/CoMMA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /stck/aremigi/CoMMA/build

# Include any dependencies generated for this target.
include Google_tests/CMakeFiles/Google_Tests_run.dir/depend.make

# Include the progress variables for this target.
include Google_tests/CMakeFiles/Google_Tests_run.dir/progress.make

# Include the compile flags for this target's objects.
include Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.o: ../Google_tests/Tests_Agglomerator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.o: ../Google_tests/Tests_Agglomerator_basic.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator_basic.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator_basic.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator_basic.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.o: ../Google_tests/Tests_Agglomerator_triconnected.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator_triconnected.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator_triconnected.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Agglomerator_triconnected.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.o: ../Google_tests/Tests_Coarse_Cell.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Coarse_Cell.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Coarse_Cell.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Coarse_Cell.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.o: ../Google_tests/Tests_Coarse_Cell_Graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Coarse_Cell_Graph.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Coarse_Cell_Graph.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Coarse_Cell_Graph.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.o: ../Google_tests/Tests_CoMMA.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_CoMMA.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_CoMMA.cpp > CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_CoMMA.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.o: ../Google_tests/Tests_Edge.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Edge.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Edge.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Edge.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.o: ../Google_tests/Tests_Dual_Graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Dual_Graph.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Dual_Graph.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Dual_Graph.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.o: ../Google_tests/Tests_First_Order_Neighbourhood.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_First_Order_Neighbourhood.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_First_Order_Neighbourhood.cpp > CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_First_Order_Neighbourhood.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.o: ../Google_tests/Tests_Seeds_Pool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Seeds_Pool.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Seeds_Pool.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Seeds_Pool.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.o: ../Google_tests/Tests_Triconnected_graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Triconnected_graph.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Triconnected_graph.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Triconnected_graph.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.o: ../Google_tests/Tests_Utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Utils.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Utils.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Utils.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.s

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.o: ../Google_tests/Tests_Util_graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.o"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.o -c /stck/aremigi/CoMMA/Google_tests/Tests_Util_graph.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.i"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /stck/aremigi/CoMMA/Google_tests/Tests_Util_graph.cpp > CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.s"
	cd /stck/aremigi/CoMMA/build/Google_tests && /opt/tools/gcc-8.3/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /stck/aremigi/CoMMA/Google_tests/Tests_Util_graph.cpp -o CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.s

# Object files for target Google_Tests_run
Google_Tests_run_OBJECTS = \
"CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.o" \
"CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.o"

# External object files for target Google_Tests_run
Google_Tests_run_EXTERNAL_OBJECTS =

Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_basic.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Agglomerator_triconnected.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Coarse_Cell_Graph.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_CoMMA.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Edge.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Dual_Graph.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_First_Order_Neighbourhood.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Seeds_Pool.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Triconnected_graph.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Utils.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/Tests_Util_graph.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/build.make
Google_tests/Google_Tests_run: CoMMA_lib/libCoMMA_lib.a
Google_tests/Google_Tests_run: lib/libgtestd.a
Google_tests/Google_Tests_run: lib/libgtest_maind.a
Google_tests/Google_Tests_run: lib/libgtestd.a
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/stck/aremigi/CoMMA/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable Google_Tests_run"
	cd /stck/aremigi/CoMMA/build/Google_tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Google_Tests_run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Google_tests/CMakeFiles/Google_Tests_run.dir/build: Google_tests/Google_Tests_run

.PHONY : Google_tests/CMakeFiles/Google_Tests_run.dir/build

Google_tests/CMakeFiles/Google_Tests_run.dir/clean:
	cd /stck/aremigi/CoMMA/build/Google_tests && $(CMAKE_COMMAND) -P CMakeFiles/Google_Tests_run.dir/cmake_clean.cmake
.PHONY : Google_tests/CMakeFiles/Google_Tests_run.dir/clean

Google_tests/CMakeFiles/Google_Tests_run.dir/depend:
	cd /stck/aremigi/CoMMA/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /stck/aremigi/CoMMA /stck/aremigi/CoMMA/Google_tests /stck/aremigi/CoMMA/build /stck/aremigi/CoMMA/build/Google_tests /stck/aremigi/CoMMA/build/Google_tests/CMakeFiles/Google_Tests_run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Google_tests/CMakeFiles/Google_Tests_run.dir/depend

