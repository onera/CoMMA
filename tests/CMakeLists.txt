
add_executable(test_structure test_structure.cpp)

add_subdirectory(Catch2)

target_link_libraries(Catch2 CoMMA_lib)
