# built from the following example:
# https://github.com/benjaminjack/python_cpp_example/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.14)

set(CMAKE_BUILD_TYPE Debug)  # Debug version


set(CMAKE_CXX_STANDARD 17)

# for linker in MAC 
# https://stackoverflow.com/questions/27508451/cmake-error-undefined-symbols-for-x86-64

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -c")

project(CoMMA_Project)

# Effective library
SET(SOURCE_DIR "CoMMA_lib")
# headers are in source dir
include_directories(${SOURCE_DIR})
set(IMPLEMENTATION
	${SOURCE_DIR}/CoMMA.h
        ${SOURCE_DIR}/Seeds_Pool.h
        ${SOURCE_DIR}/Dual_Graph.h
        ${SOURCE_DIR}/Coarse_Cell.h
        ${SOURCE_DIR}/Agglomerator.h
        ${SOURCE_DIR}/Coarse_Cell_Graph.h
        ${SOURCE_DIR}/Util.h
        ${SOURCE_DIR}/First_Order_Neighbourhood.h
        ${SOURCE_DIR}/Edge.h
        ${SOURCE_DIR}/CompStruct.h
        ${SOURCE_DIR}/Triconnected_graph.h
        ${SOURCE_DIR}/Util_graph.h
        ${SOURCE_DIR}/CoMMATypes.h
	${SOURCE_DIR}/templateHelpers.h
	)
set(INTERFACCIA
        ${SOURCE_DIR}/CoMMA.cpp
	)
set(SOURCE_FILES
	${SOURCE_DIR}/Seeds_Pool.cpp
        ${SOURCE_DIR}/Dual_Graph.cpp
        ${SOURCE_DIR}/Coarse_Cell.cpp
        ${SOURCE_DIR}/Agglomerator.cpp
        ${SOURCE_DIR}/Coarse_Cell_Graph.cpp
        ${SOURCE_DIR}/Util.cpp
        ${SOURCE_DIR}/First_Order_Neighbourhood.cpp
        ${SOURCE_DIR}/Edge.cpp
        ${SOURCE_DIR}/CompStruct.cpp
        ${SOURCE_DIR}/Triconnected_graph.cpp
        ${SOURCE_DIR}/Util_graph.cpp
	)



SET(TEST_DIR "tests")

SET(TESTING_FILES
	${TEST_DIR}/test_structure.cpp)

set(TESTS ${IMPLEMENTATION}
	${TESTING_FILES})

# Generate a test executable
include_directories(Catch2/single_include/Catch2)

add_executable("Comma_test" ${TESTS})

add_subdirectory(pybind11)
pybind11_add_module(CoMMA ${IMPLEMENTATION} ${SOURCE_FILES})

