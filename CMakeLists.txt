CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(Katarenga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

## ZeroMQ dependency
find_package(ZMQ MODULE QUIET)
if(NOT ZMQ_FOUND)
    find_package(ZMQ CONFIG REQUIRED)
endif()

## Docopt dependency
find_package(docopt MODULE QUIET)
if(NOT DOCOPT_FOUND)
    find_package(docopt CONFIG REQUIRED)
endif()

## GLTK dependency
find_package(GLTK MODULE QUIET)
if(NOT GLTK_FOUND)
    find_package(GLTK CONFIG REQUIRED)
endif()

file(GLOB SOURCE_FILES
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${_ZMQ_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${_ZMQ_LIBRARY}
    docopt
    GLTK
)
