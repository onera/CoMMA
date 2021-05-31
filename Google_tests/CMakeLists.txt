

project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(Google_Tests_run
        Tests_Utils.cpp
        Tests_Agglomerator_isotropic.cpp
        Tests_Coarse_Cell.cpp
        Tests_Coarse_Cell_Graph.cpp
        Tests_Agglomerator.cpp
        Tests_Dual_Graph.cpp
        Tests_Seeds_Pool.cpp
        Tests_CoMMA.cpp
        Tests_Edge.cpp
        Tests_Triconnected_graph.cpp
        Tests_First_Order_Neighbourhood.cpp)

target_link_libraries(Google_Tests_run CoMMA_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)