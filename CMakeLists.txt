# built from the following example:
# https://github.com/benjaminjack/python_cpp_example/blob/master/CMakeLists.txt

###############################   CMAKE VERSION and FLAGS ##########################

cmake_minimum_required(VERSION 3.15)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type received, using Debug")
  set(CMAKE_BUILD_TYPE Debug)  # Debug version
endif()

set(CMAKE_CXX_STANDARD 17)
# for linker in MAC read:
# https://github.com/catchorg/Catch2/issues/1204

###################################  PROJECT #################################
project(CoMMA
  VERSION 1.3.2
  DESCRIPTION "Geometric agglomerator for unstructured grids"
  HOMEPAGE_URL "https://github.com/onera/CoMMA"
  LANGUAGES CXX
)
# Target name
set(CoMMA_LIB "${PROJECT_NAME}_lib")

# https://stackoverflow.com/questions/57860026/cmake-override-variable-set-in-cmakelists-txt-via-commandline
# For the current application, better to use an `option`:
# https://stackoverflow.com/questions/36358217/what-is-the-difference-between-option-and-set-cache-bool-for-a-cmake-variabl
#set(BUILD_TESTS On CACHE BOOL "Build tests")
option(BUILD_TESTS "Build tests" ON)
option(COVERAGE "Build tests with coverage support" OFF)
# Target name
set(CoMMA_TEST "${PROJECT_NAME}_test")

#set(BUILD_PYTHON_BINDINGS On CACHE BOOL "Build python bindings via pybind11")
option(BUILD_PYTHON_BINDINGS "Build python bindings via pybind11" ON)
# Target name
set(CoMMA_PYTHON "${PROJECT_NAME}")

# https://vicrucann.github.io/tutorials/quick-cmake-doxygen/ and
# https://stackoverflow.com/questions/34878276/build-doxygen-from-cmake-script
option(BUILD_DOC "Build documentation (with Doxygen)" OFF)

option(PKGCONFIG_SUPPORT "Prepare and install file to be used with pkg-config" OFF)

if ( CODAFLAGS )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  # We just do not set the optimization flags nor the parallelism-related ones
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-finite-math-only -fno-math-errno -fvisibility-inlines-hidden -Wno-int-in-bool-context -Wno-misleading-indentation -Wno-deprecated-declarations")
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "^Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-finite-math-only -fno-math-errno -fvisibility-inlines-hidden -Wno-absolute-value -Wno-deprecated-register -Wno-unused-lambda-capture")
  endif()
  message(STATUS "Added CODA-like extra flags. Current flags are: ${CMAKE_CXX_FLAGS}")
endif()

# Options for CoMMA types
set(INT_T "int" CACHE STRING "Type for standard integers")
set(INDEX_T "unsigned long" CACHE STRING "Type for integers for indices")
set(REAL_T "double" CACHE STRING "Type for real values")
function(check_int_type int_type label)
  # Regex inspired by https://stackoverflow.com/a/12993881/12152457
  if ( NOT int_type MATCHES "^((un)?signed *)?((long *)*|short *)?(int *)?$"
      AND NOT int_type MATCHES "^u?int((_(fast|least))?(8|16|32|64)|(max|ptr))_t$" )
    message(FATAL_ERROR "Invalid ${label} \"${int_type}\": Only integer types accepted")
  endif()
  string(TOUPPER ${label} out_label)
  message(STATUS "${out_label} set to \"${int_type}\"")
endfunction()
check_int_type(${INT_T} "integer type")
check_int_type(${INDEX_T} "index type")
# Fixed width floating points only in C++23 https://en.cppreference.com/w/cpp/types/floating-point
if ( NOT REAL_T STREQUAL "float" AND NOT REAL_T MATCHES "^(long )?double$" )
  message(FATAL_ERROR "Invalid real type \"${REAL_T}\": Only \"(long )double\" and \"float\" accepted")
endif()
message(STATUS "REAL TYPE set to \"${REAL_T}\"")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Effective library

SET(INCLUDE_DIR "include")
SET(SOURCE_DIR "src")

add_library(${CoMMA_LIB} INTERFACE)

# Add headers
set(ADDITIONAL_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
configure_file(config_files/CoMMAConfig.h.in ${ADDITIONAL_INCLUDE_DIR}/${PROJECT_NAME}/CoMMAConfig.h @ONLY)
target_include_directories(${CoMMA_LIB} INTERFACE ${INCLUDE_DIR} ${ADDITIONAL_INCLUDE_DIR})

SET(PROFILING_DIR "misc_scripts")

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
  ${PROFILING_DIR}/DualGraphExamples.h
  ${PROFILING_DIR}/test_profile.cpp
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
  ${TEST_DIR}/DualGraphExamples.h
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
#############################  TEST with CATCH2 BUILDING #########################
# Why Catch2? Read: https://snorristurluson.github.io/Catch2/

# Generate a test executable with coverage report
if ( BUILD_TESTS )
  message(STATUS "Tests enabled")
  add_subdirectory(Catch2)
  add_executable(${CoMMA_TEST} ${TESTS})
  target_link_libraries(${CoMMA_TEST}
    PUBLIC ${CoMMA_LIB}
    PRIVATE Catch2::Catch2
  )
  if ( COVERAGE )
    message(STATUS "Coverage support enabled")
    target_compile_options(${CoMMA_TEST} PUBLIC "--coverage")
    target_link_libraries(${CoMMA_TEST} PRIVATE gcov)
  endif()

  enable_testing()
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Catch2/contrib)
  include(CTest)
  include(Catch)
  catch_discover_tests(${CoMMA_TEST})
endif()
#target_link_libraries(${CoMMA_TEST} Boost)
#set_source_files_properties(${CoMMA_TEST} PROPERTIES COMPILE_FLAGS "--coverage")
######################## Pybind11 bindings ####################################
if ( BUILD_PYTHON_BINDINGS )
  # We use pybind11, see https://pybind11.readthedocs.io/en/latest/compiling.html#building-with-cmake
  message(STATUS "Python bindings enabled")
  # find the Python interpreter (including pybind might lead to other find files being used)
  find_package(Python COMPONENTS Interpreter Development REQUIRED)
  # Without the following, CMake and pybind seems to find different versions of python
  # https://pybind11.readthedocs.io/en/stable/faq.html#inconsistent-detection-of-python-version-in-cmake-and-pybind11
  # option(PYBIND11_FINDPYTHON "Use pybind11 to find python" ON)
  add_subdirectory(pybind11)
  pybind11_add_module(${CoMMA_PYTHON} ${PYTHONBIND})
  target_link_libraries(${CoMMA_PYTHON} PUBLIC ${CoMMA_LIB})
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

# ${INCLUDE_DIR} (no slash) considers the directory itself
# ${INCLUDE_DIR}/ (with slash) considers its files only
install(DIRECTORY ${INCLUDE_DIR}/
  DIRECTORY ${ADDITIONAL_INCLUDE_DIR}/
  TYPE INCLUDE
  FILES_MATCHING PATTERN "*.h"
  REGEX "deprecated" EXCLUDE
)
install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})

if ( PKGCONFIG_SUPPORT )
  configure_file(config_files/comma.pc.in comma.pc @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/comma.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

if ( BUILD_PYTHON_BINDINGS )
  set(PYTHON_SUB_DIR "python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/site-packages")
  # Install generated module
  install(TARGETS ${CoMMA_PYTHON}
    COMPONENT python
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PYTHON_SUB_DIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/${PYTHON_SUB_DIR}"
  )

  # Install examples
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/scripts/
    DESTINATION ${CMAKE_INSTALL_PREFIX}/examples/${PROJECT_NAME}
    FILES_MATCHING
    PATTERN "*.py"
    PATTERN "*.md"
    REGEX "deprecated" EXCLUDE
    REGEX "cache" EXCLUDE
  )
endif()

if ( BUILD_DOC )
  # Instead of "TYPE DOC" could have use ${CMAKE_INSTALL_DOCDIR} or similarly ${CMAKE_INSTALL_FULL_DOCDIR}
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/documentation/html TYPE DOC)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Documentation/CoMMA_user_manual.pdf TYPE DOC)
endif()
