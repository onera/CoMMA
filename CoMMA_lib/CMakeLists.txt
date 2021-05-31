cmake_minimum_required(VERSION 3.14)
project(CoMMA_lib)

set(CMAKE_CXX_STANDARD 11)

set(HEADER_FILES
        Agglomerator_anisotropic.h
        Agglomerator_Isotropic.h
        Seeds_Pool.h
        Dual_Graph.h
        Coarse_Cell.h
        Agglomerator.h
        Coarse_Cell_Graph.h
        Util.h
        First_Order_Neighbourhood.h Edge.cpp Edge.h CompStruct.cpp CompStruct.h Triconnected_graph.cpp Triconnected_graph.h)

set(SOURCE_FILES
        Agglomerator_anisotropic.cpp
        Agglomerator_Isotropic.cpp
        Seeds_Pool.cpp
        Dual_Graph.cpp
        Coarse_Cell.cpp
        Agglomerator.cpp
        Coarse_Cell_Graph.cpp
        Util.cpp
        First_Order_Neighbourhood.cpp Edge.cpp Edge.h CompStruct.cpp CompStruct.h Triconnected_graph.cpp Triconnected_graph.h)

add_library(CoMMA_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
#add_executable(toto ${SOURCE_FILES})
#add_executable(untitled ${SOURCE_FILES} ${gtest_SOURCE_DIR})
#target_link_libraries(untitled gtest gtest_main)








