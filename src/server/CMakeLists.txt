cmake_minimum_required(VERSION 3.2)
project(katarenga_server)

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

set(HEADER_FILES
    Board.hpp
    Cell.hpp
    Piece.hpp
    server.hpp
    server_utils.hpp
)

set(SOURCE_FILES
    Board.cpp
    Cell.cpp
    Piece.cpp
    server.cpp
    server_utils.cpp
    katarenga_server.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ZMQPP
    docopt
    message_utils
)
