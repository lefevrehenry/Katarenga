cmake_minimum_required(VERSION 3.2)
project(katarenga_server)

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
