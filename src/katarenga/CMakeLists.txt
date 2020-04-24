cmake_minimum_required(VERSION 3.2)
project(katarenga)

## Threads dependency
find_package(Threads REQUIRED)

## GLTK dependency
find_package(GLTK MODULE QUIET)
if(NOT GLTK_FOUND)
    find_package(GLTK CONFIG REQUIRED)
endif()
get_target_property(_GLTK_DIR GLTK INTERFACE_INCLUDE_DIRECTORIES)
message("-- Found GLTK: ${_GLTK_DIR}")

file(GLOB SOURCE_FILES
    "*.cpp"
    "*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    katarenga_common
    Threads::Threads
    ZMQPP
    docopt
    GLTK
)
