cmake_minimum_required(VERSION 3.14)

set(CMAKE_BUILD_TYPE Debug)  # Debug version


set(CMAKE_CXX_STANDARD 14)


project(CoMMA_Project)

add_subdirectory(CoMMA_lib)
get_directory_property(HEADER_FILES DIRECTORY CoMMA_lib DEFINITION HEADER_FILES)
get_directory_property(SOURCE_FILES DIRECTORY CoMMA_lib DEFINITION SOURCE_FILES)
add_subdirectory(pybind11)
pybind11_add_module(CoMMA ${SOURCE_FILES} ${HEADER_FILES})



