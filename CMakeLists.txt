cmake_minimum_required(VERSION 3.2)
project(katarenga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

## Threads dependency
find_package(Threads REQUIRED)

## ZeroMQ dependency
find_package(ZMQPP MODULE QUIET)
if(NOT ZMQPP_FOUND)
    find_package(ZMQPP CONFIG REQUIRED)
endif()
get_target_property(_ZMQPP_DIR ZMQPP INTERFACE_INCLUDE_DIRECTORIES)
message("-- Found ZMQPP: ${_ZMQPP_DIR}")

## Docopt dependency
find_package(docopt MODULE QUIET)
if(NOT DOCOPT_FOUND)
    find_package(docopt CONFIG REQUIRED)
endif()
get_target_property(_DOCOPT_DIR docopt INTERFACE_INCLUDE_DIRECTORIES)
message("-- Found docopt: ${_DOCOPT_DIR}")

## GLTK dependency
find_package(GLTK MODULE QUIET)
if(NOT GLTK_FOUND)
    find_package(GLTK CONFIG REQUIRED)
endif()
get_target_property(_GLTK_DIR GLTK INTERFACE_INCLUDE_DIRECTORIES)
message("-- Found GLTK: ${_GLTK_DIR}")

file(GLOB SOURCE_FILES
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Threads::Threads
    ZMQPP
    docopt
    GLTK
)
