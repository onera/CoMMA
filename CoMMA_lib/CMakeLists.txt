cmake_minimum_required(VERSION 3.14)
project(CoMMA_lib)

set(CMAKE_CXX_STANDARD 11)

set(HEADER_FILES
        Agglomerator.h
        Agglomerator_Isotropic.h
        tests.h
        tests_agglomerate.h
        )

set(SOURCE_FILES
        Agglomerator.cpp
        Agglomerator_Isotropic.cpp
        tests.cpp
        tests_agglomerate.cpp)

add_library(CoMMA_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
#add_executable(toto ${SOURCE_FILES})
#add_executable(untitled ${SOURCE_FILES} ${gtest_SOURCE_DIR})
#target_link_libraries(untitled gtest gtest_main)








