CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(Katarenga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread -lzmq")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

## ZeroMQ dependency
find_package(ZMQ MODULE QUIET)
if(NOT ZMQ_FOUND)
    find_package(ZMQ CONFIG REQUIRED)
endif()
get_target_property(_ZMQ_DIR ZMQ INTERFACE_INCLUDE_DIRECTORIES)
message("-- Found ZMQ: ${_ZMQ_DIR}")

## Docopt dependency
find_package(docopt MODULE QUIET)
if(NOT DOCOPT_FOUND)
    find_package(docopt CONFIG REQUIRED)
endif()
get_target_property(_DOCOPT_DIR docopt INTERFACE_INCLUDE_DIRECTORIES)
message("-- Found ZMQ: ${_DOCOPT_DIR}")

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
    docopt
    GLTK
)
