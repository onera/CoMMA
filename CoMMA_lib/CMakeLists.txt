set(HEADER_FILES
	CoMMA_lib/Seeds_Pool.h
        CoMMA_lib/Dual_Graph.h
        CoMMA_lib/Coarse_Cell.h
        CoMMA_lib/Agglomerator.h
        CoMMA_lib/Coarse_Cell_Graph.h
        CoMMA_lib/Util.h
        CoMMA_lib/First_Order_Neighbourhood.h
        CoMMA_lib/Edge.h
        CoMMA_lib/CompStruct.h
        CoMMA_lib/Triconnected_graph.h
        CoMMA_lib/Util_graph.h
        CoMMA_lib/CoMMATypes.h
	CoMMA_lib/templateHelpers.h
	CoMMA_lib/CoMMA.h
	)

set(SOURCE_FILES
	CoMMA_lib/Seeds_Pool.cpp
        CoMMA_lib/Dual_Graph.cpp
        CoMMA_lib/Coarse_Cell.cpp
        CoMMA_lib/Agglomerator.cpp
        CoMMA_lib/Coarse_Cell_Graph.cpp
        CoMMA_lib/Util.cpp
        CoMMA_lib/First_Order_Neighbourhood.cpp
        CoMMA_lib/Edge.cpp
        CoMMA_lib/CompStruct.cpp
        CoMMA_lib/Triconnected_graph.cpp
        CoMMA_lib/Util_graph.cpp
        CoMMA_lib/CoMMA.cpp
	)


#set(CMAKE_BUILD_TYPE Debug)

#add_library(CoMMA_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

#add_executable(toto ${SOURCE_FILES})
#add_executable(untitled ${SOURCE_FILES} ${gtest_SOURCE_DIR})
#target_link_libraries(untitled gtest gtest_main)
