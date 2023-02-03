cmake_minimum_required(VERSION 3.2)
project(katarenga_common)

set(HEADER_FILES
    src/common/common_utils.hpp

    src/common/board/Board.hpp
    src/common/board/Cell.hpp
    src/common/board/board_utils.hpp

    src/common/engines/pair_engine.hpp
    src/common/engines/reply_engine.hpp
    src/common/engines/request_engine.hpp

    src/common/messages/message.hpp
    src/common/messages/messages.hpp

    src/common/sockets/abstract_socket.hpp
    src/common/sockets/pair_socket.hpp
    src/common/sockets/reply_socket.hpp
    src/common/sockets/request_socket.hpp
)

set(SOURCE_FILES
    src/common/common_utils.cpp

    src/common/board/Board.cpp
    src/common/board/Cell.cpp
    src/common/board/board_utils.cpp

    src/common/engines/pair_engine.cpp
    src/common/engines/reply_engine.cpp
    src/common/engines/request_engine.cpp

    src/common/messages/message.cpp
    src/common/messages/messages.hpp

    src/common/sockets/abstract_socket.cpp
    src/common/sockets/pair_socket.cpp
    src/common/sockets/reply_socket.cpp
    src/common/sockets/request_socket.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ZMQPP
)
