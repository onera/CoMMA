

project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(Google_Tests_run
        Tests_Utils.cpp
        Tests_Agglomerator_isotropic.cpp
        Tests_CC.cpp
        Tests_CoarseCellGraph.cpp
        Tests_DualGraph.cpp)

target_link_libraries(Google_Tests_run CoMMA_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)