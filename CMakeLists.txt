cmake_minimum_required(VERSION 2.8)
# include(ExternalProject)
project (hdf2vis)
message ("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")

option(build_tests "Build the test suite." OFF)
add_definitions(-DH5_USE_BOOST)

# Set CXX flags
# set(CMAKE_CXX_COMPILER "/opt/intel/bin/icpc")
set(CMAKE_CXX_STANDARD 14)
set(CXX_EXTRA_FLAGS "-Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++14 -pthread -O3 -g -DNDEBUG -Wall -Wextra -fPIC -march=native ${CXX_EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++14 -pthread -g3 -O0 -Wall -Wextra -fPIC ${CXX_EXTRA_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(HDF5_USE_STATIC_LIBRARIES)

find_package(HDF5 REQUIRED COMPONENTS C CXX)
find_package(Boost COMPONENTS system filesystem REQUIRED)

set(FMT_INCLUDE_DIR deps/fmt)
set(HIGHFIVE_INCLUDE_DIR deps/highfive/include)
include_directories(${FMT_INCLUDE_DIR} ${HIGHFIVE_INCLUDE_DIR} ${Boost_iNCLUDE_DIR} include)

add_subdirectory(deps/fmt)
add_subdirectory(src)

# This is to get all header files recognized by QtCreator
file(GLOB_RECURSE headers "include/*.h" "include/*/*.h" "include/*.hpp" "include/*/*.hpp")
add_custom_target(myIncludes SOURCES ${headers})
