cmake_minimum_required(VERSION 3.14)
project(CoMMA_lib)
set(CMAKE_CXX_STANDARD 11)

set(HEADER_FILES
        Seeds_Pool.h
        Dual_Graph.h
        Coarse_Cell.h
        Agglomerator.h
        Coarse_Cell_Graph.h
        Util.h
        First_Order_Neighbourhood.h
        Edge.h
        CompStruct.h
        Triconnected_graph.h
        Util_graph.h
        CoMMATypes.h
	templateHelpers.h
	Utils_Agglomerator_Anisotropic.h
	CoMMA.h
	)

set(SOURCE_FILES
        Seeds_Pool.cpp
        Dual_Graph.cpp
        Coarse_Cell.cpp
        Agglomerator.cpp
        Coarse_Cell_Graph.cpp
        Util.cpp
        First_Order_Neighbourhood.cpp
        Edge.cpp
        CompStruct.cpp
        Triconnected_graph.cpp
        Util_graph.cpp
	Utils_Agglomerator_Anisotropic.cpp
        CoMMA.cpp
	)


#set(CMAKE_BUILD_TYPE Debug)
add_subdirectory(pybind11)
pybind11_add_module(CoMMA ${SOURCE_FILES} ${HEADER_FILES})

#add_library(CoMMA_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

#add_executable(toto ${SOURCE_FILES})
#add_executable(untitled ${SOURCE_FILES} ${gtest_SOURCE_DIR})
#target_link_libraries(untitled gtest gtest_main)
