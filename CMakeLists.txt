cmake_minimum_required(VERSION 3.8)
project(CoMMA)

set(CMAKE_CXX_STANDARD 11)


#set(gtest_SOURCE_DIR googletest-master/googletest)
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


set(SOURCE_FILES main.cpp Agglomerator.cpp Agglomerator.h Agglomerator_Isotropic.cpp Agglomerator_Isotropic.h tests.cpp tests.h tests_agglomerate.cpp tests_agglomerate.h)
add_executable(toto ${SOURCE_FILES})
#add_executable(untitled ${SOURCE_FILES} ${gtest_SOURCE_DIR})
#target_link_libraries(untitled gtest gtest_main)



