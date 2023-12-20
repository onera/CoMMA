# built from the following example:
# https://github.com/benjaminjack/python_cpp_example/blob/master/CMakeLists.txt

###############################   CMAKE VERSION and FLAGS ##########################

cmake_minimum_required(VERSION 3.14)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type received, using Debug")
  set(CMAKE_BUILD_TYPE Debug)  # Debug version
endif()

set(CMAKE_CXX_STANDARD 17)
# for linker in MAC read:
# https://github.com/catchorg/Catch2/issues/1204

###################################  PROJECT #################################
project(CoMMA VERSION 1.3)

# https://stackoverflow.com/questions/57860026/cmake-override-variable-set-in-cmakelists-txt-via-commandline
# For the current application, better to use an `option`:
# https://stackoverflow.com/questions/36358217/what-is-the-difference-between-option-and-set-cache-bool-for-a-cmake-variabl
#set(BUILD_TESTS On CACHE BOOL "Build tests")
option(BUILD_TESTS "Build tests" ON)
set(CoMMA_TEST "CoMMA_test")
option(COVERAGE "Build tests with coverage support" OFF)

#set(BUILD_PYTHON_BINDINGS On CACHE BOOL "Build python bindings via pybind11")
option(BUILD_PYTHON_BINDINGS "Build python bindings via pybind11" ON)

# https://vicrucann.github.io/tutorials/quick-cmake-doxygen/ and
# https://stackoverflow.com/questions/34878276/build-doxygen-from-cmake-script
option(BUILD_DOC "Build documentation (with Doxygen)" OFF)

if ( CODAFLAGS )
   # We just do not set the optimization flags nor the parallelism-related ones
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fno-finite-math-only -fno-math-errno -fvisibility-inlines-hidden -Wno-int-in-bool-context -Wno-misleading-indentation -Wno-deprecated-declarations")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fvisibility-inlines-hidden")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-finite-math-only -fno-math-errno -fvisibility-inlines-hidden -Wall -Wno-absolute-value -Wno-deprecated-register -Wno-unused-lambda-capture")
  endif()
  message(STATUS "Added CODA-like extra flags. Current flags are: ${CMAKE_CXX_FLAGS}")
endif()

# Effective library

SET(SOURCE_DIR "CoMMA_lib")

# headers are in source dir

include_directories(${SOURCE_DIR})

SET(PROFILING_DIR "tests")

SET(TEST_DIR "tests")

set(PYTHONBIND
    ${SOURCE_DIR}/CoMMA.cpp
)

set(PROFILING
    perfetto/sdk/perfetto.h
    ${PROFILING_DIR}/profiling/trace_categories.h
    ${PROFILING_DIR}/profiling/trace_categories.cpp
    ${PROFILING_DIR}/header_profile.h
)

set(PROFILING_SOURCE
    ${PROFILING_DIR}/input/DualGPy.h
    ${PROFILING_DIR}/test_profile.cpp
)

set(PROFILING_PERF
    ${PROFILING_DIR}/input/DualGPy.h
    ${PROFILING_DIR}/test_perf.cpp
)

SET(TESTING_FILES
    ${TEST_DIR}/test_anisoagglo.cpp
    ${TEST_DIR}/test_correction.cpp
    ${TEST_DIR}/test_deprecated.cpp
    ${TEST_DIR}/test_graph.cpp
    ${TEST_DIR}/test_isoagglo.cpp
    ${TEST_DIR}/test_neighbourhood.cpp
    ${TEST_DIR}/test_seedspool.cpp
    ${TEST_DIR}/test_structure.cpp
    ${TEST_DIR}/test_utils.cpp
)

SET(TESTS
    ${TEST_DIR}/input/DualGPy.h
    ${TEST_DIR}/test_defs.h
    ${TESTING_FILES}
)

###################### Perfetto Settings #####################
#find_package(Threads)
### Define a static library for Perfetto.
#include_directories(perfetto/sdk)
#add_library(perfetto STATIC perfetto/sdk/perfetto.cc)
### Link the library to your main executable.
#add_executable("Profiling" ${PROFILING_SOURCE} ${PROFILING} ${IMPLEMENTATION})
#target_link_libraries(Profiling perfetto ${CMAKE_THREAD_LIBS_INIT})
#
######################### Profiling Perf ########################################
#
#add_executable("Profiling_perf" ${PROFILING_PERF} ${IMPLEMENTATION})
#############################  TEST with CATCH2 BUILDING #########################
# Why Catch2? Read: https://snorristurluson.github.io/Catch2/

# Generate a test executable with covergae report
if ( BUILD_TESTS )
  message(STATUS "Tests enabled")
  include_directories(Catch2/single_include)
  if ( COVERAGE )
    message(STATUS "Coverage support enabled")
    add_compile_options("--coverage")
  endif()
  add_executable(${CoMMA_TEST} ${TESTS})
  if ( COVERAGE )
    target_link_libraries(${CoMMA_TEST} gcov)
  endif()

  enable_testing()
  add_test(NAME ${CoMMA_TEST}_global COMMAND ${CoMMA_TEST})
endif()
#target_link_libraries(${CoMMA_TEST} Boost)
#set_source_files_properties(${CoMMA_TEST} PROPERTIES COMPILE_FLAGS "--coverage")
######################## Pybind11 bindings ####################################
if ( BUILD_PYTHON_BINDINGS )
  message(STATUS "Python bindings enabled")
  add_subdirectory(pybind11)
  pybind11_add_module(${PROJECT_NAME} ${PYTHONBIND})
endif()

if ( BUILD_DOC )
  message(STATUS "Documentation enabled")
  find_package(Doxygen)
  if ( NOT DOXYGEN_FOUND )
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

  # We add it to all-target as well
  add_custom_target(doc
    ALL
    COMMAND ${DOXYGEN_EXECUTABLE} Documentation/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating documentation with Doxygen"
  )
endif()

# installation
include(GNUInstallDirs)
# configure_file(comma.pc.in comma.pc @ONLY)

# ${SOURCE_DIR} (no slash) considers the directory itself
# ${SOURCE_DIR}/ (with slash) considers its files only
install(DIRECTORY ${SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
  REGEX "deprecated" EXCLUDE
)
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/comma.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

if ( BUILD_PYTHON_BINDINGS )
  install(TARGETS ${PROJECT_NAME}
    COMPONENT python
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )
endif()

if ( BUILD_DOC )
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/documentation/html TYPE DOC)
  install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Documentation/CoMMA_user_manual.pdf
    TYPE DOC
  )
endif()
