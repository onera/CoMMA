cmake_minimum_required(VERSION 3.14)

set(CMAKE_BUILD_TYPE Debug)  # Debug version

project(CoMMA_Project)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp )

add_executable(CoMMA_run ${SOURCE_FILES})

add_subdirectory(CoMMA_lib)
add_subdirectory(Google_tests)

include_directories(CoMMA_lib)

target_link_libraries(CoMMA_run CoMMA_lib)
