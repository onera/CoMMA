cmake_minimum_required(VERSION 3.14)

set(CMAKE_BUILD_TYPE Debug)  # Debug version

project(CoMMA_Project)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory(CoMMA_lib)

include_directories(CoMMA_lib)


