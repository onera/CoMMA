# built from the following example:
# https://github.com/benjaminjack/python_cpp_example/blob/master/CMakeLists.txt

###############################   CMAKE VERSION and FLAFS ##########################


cmake_minimum_required(VERSION 3.14)

set(CMAKE_BUILD_TYPE Debug)  # Debug version


set(CMAKE_CXX_STANDARD 17)
# for linker in MAC read: 
# https://github.com/catchorg/Catch2/issues/1204

###################################  PROJECT #################################
project(CoMMA_Project)

# Effective library

SET(SOURCE_DIR "CoMMA_lib")


# headers are in source dir

include_directories(${SOURCE_DIR})

SET(PROFILING_DIR "tests")
SET(TEST_DIR "tests")
SET(IMPLEMENTATION
	${SOURCE_DIR}/CoMMA.h
        ${SOURCE_DIR}/Seeds_Pool.h
        ${SOURCE_DIR}/Dual_Graph.h
        ${SOURCE_DIR}/Coarse_Cell.h
        ${SOURCE_DIR}/Agglomerator.h
        ${SOURCE_DIR}/Coarse_Cell_Container.h
        ${SOURCE_DIR}/Util.h
        ${SOURCE_DIR}/Queue.h
        ${SOURCE_DIR}/First_Order_Neighbourhood.h
        ${SOURCE_DIR}/Tree.h
        ${SOURCE_DIR}/CoMMATypes.h
	${SOURCE_DIR}/templateHelpers.h
	${SOURCE_DIR}/Bimap.h
	)
set(INTERFACCIA
        ${SOURCE_DIR}/CoMMA.cpp
	)
set(SOURCE_FILES
#        ${SOURCE_DIR}/Coarse_Cell_Graph.cpp
        ${SOURCE_DIR}/Tree.cpp
	)
set(PROFILING 
	perfetto/sdk/perfetto.h
	${PROFILING_DIR}/profiling/trace_categories.h
	${PROFILING_DIR}/profiling/trace_categories.cpp
	${PROFILING_DIR}/header_profile.h
	)

set(PROFILING_SOURCE 
	${PROFILING_DIR}/input/DualGPy.h	
	${PROFILING_DIR}/test_profile.cpp
   )

set(PROFILING_PERF 
	${PROFILING_DIR}/input/DualGPy.h	
	${PROFILING_DIR}/test_perf.cpp
   )

SET(TESTING_FILES
  ${TEST_DIR}/test_types.h
	${TEST_DIR}/test_structure.cpp
	)

SET(TESTS ${IMPLEMENTATION}
        ${SOURCE_FILES}
	${TEST_DIR}/input/DualGPy.h
	${TESTING_FILES})




###################### Perfetto Settings #####################
#find_package(Threads)
### Define a static library for Perfetto.
#include_directories(perfetto/sdk)
#add_library(perfetto STATIC perfetto/sdk/perfetto.cc)
### Link the library to your main executable.
#add_executable("Profiling" ${PROFILING_SOURCE} ${PROFILING} ${IMPLEMENTATION} ${SOURCE_FILES})
#target_link_libraries(Profiling perfetto ${CMAKE_THREAD_LIBS_INIT})
#
######################### Profiling Perf ########################################
#
#add_executable("Profiling_perf" ${PROFILING_PERF} ${IMPLEMENTATION} ${SOURCE_FILES})
#############################  TEST with CATCH2 BUILDING #########################
# Why Catch2? Read: https://snorristurluson.github.io/Catch2/


# Generate a test executable
include_directories(Catch2/single_include)
add_compile_options("--coverage")
add_executable("Comma_test" ${TESTS})
target_link_libraries(Comma_test gcov)
#target_link_libraries(Comma_test Boost)
#set_source_files_properties( Comma_test PROPERTIES COMPILE_FLAGS "--coverage" )
######################## Pybind11 bindings ####################################
add_subdirectory(pybind11)
pybind11_add_module(CoMMA ${IMPLEMENTATION} ${INTERFACCIA} ${SOURCE_FILES})


