# built from the following example:
# https://github.com/benjaminjack/python_cpp_example/blob/master/CMakeLists.txt

###############################   CMAKE VERSION and FLAGS ##########################

cmake_minimum_required(VERSION 3.14)

set(CMAKE_BUILD_TYPE Debug)  # Debug version

set(CMAKE_CXX_STANDARD 17)
# for linker in MAC read:
# https://github.com/catchorg/Catch2/issues/1204

###################################  PROJECT #################################
project(CoMMA_Project)

if ( CODAFLAGS )
   # We just do not set the optimization flags nor the parallelism-related ones
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-finite-math-only -fno-math-errno -fvisibility-inlines-hidden -Wno-int-in-bool-context -Wno-misleading-indentation -Wno-deprecated-declarations")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fvisibility-inlines-hidden")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-finite-math-only -fno-math-errno -fvisibility-inlines-hidden -Wall -Wno-absolute-value -Wno-deprecated-register -Wno-unused-lambda-capture")
  endif()
  message("Added CODA-like extra flags. Current flags are: ${CMAKE_CXX_FLAGS}")
endif()

# Effective library

SET(SOURCE_DIR "CoMMA_lib")

# headers are in source dir

include_directories(${SOURCE_DIR})

SET(PROFILING_DIR "tests")

SET(TEST_DIR "tests")

SET(DEPRECATED_DIR ${SOURCE_DIR}/deprecated)

set(DEPRECATED_SRC
    ${DEPRECATED_DIR}/Bimap.h
    ${DEPRECATED_DIR}/Queue.h
    ${DEPRECATED_DIR}/Tree.h
    ${DEPRECATED_DIR}/Priority_Pair.h
    ${DEPRECATED_DIR}/templateHelpers.h
)

set(PYTHONBIND
    ${SOURCE_DIR}/CoMMA.cpp
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
    ${TEST_DIR}/test_structure.cpp
)

SET(TESTS
    ${DEPRECATED_SRC}
    ${TEST_DIR}/input/DualGPy.h
    ${TESTING_FILES}
)

###################### Perfetto Settings #####################
#find_package(Threads)
### Define a static library for Perfetto.
#include_directories(perfetto/sdk)
#add_library(perfetto STATIC perfetto/sdk/perfetto.cc)
### Link the library to your main executable.
#add_executable("Profiling" ${PROFILING_SOURCE} ${PROFILING} ${IMPLEMENTATION})
#target_link_libraries(Profiling perfetto ${CMAKE_THREAD_LIBS_INIT})
#
######################### Profiling Perf ########################################
#
#add_executable("Profiling_perf" ${PROFILING_PERF} ${IMPLEMENTATION})
#############################  TEST with CATCH2 BUILDING #########################
# Why Catch2? Read: https://snorristurluson.github.io/Catch2/


# Generate a test executable with covergae report
include_directories(Catch2/single_include)
add_compile_options("--coverage")
add_executable("Comma_test" ${TESTS})
target_link_libraries(Comma_test gcov)
#target_link_libraries(Comma_test Boost)
#set_source_files_properties( Comma_test PROPERTIES COMPILE_FLAGS "--coverage" )
######################## Pybind11 bindings ####################################
add_subdirectory(pybind11)
pybind11_add_module(CoMMA ${PYTHONBIND})
