

project(Google_tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ./tests/0_Inputs)


add_executable(Google_Tests_run

        Tests_Agglomerator.cpp
        Tests_Agglomerator_basic.cpp
        Tests_Agglomerator_triconnected.cpp
        Tests_Agglomerator.cpp
#        Tests_Agglomerator_isotropic.cpp
        Tests_Coarse_Cell.cpp
        Tests_Coarse_Cell_Graph.cpp
        Tests_CoMMA.cpp
        Tests_Edge.cpp
        Tests_Dual_Graph.cpp
        Tests_First_Order_Neighbourhood.cpp
        Tests_Seeds_Pool.cpp
        Tests_Triconnected_graph.cpp
        Tests_Utils.cpp
        Tests_Util_graph.cpp
        )

target_link_libraries(Google_Tests_run CoMMA_lib)

target_link_libraries(Google_Tests_run gtest gtest_main)