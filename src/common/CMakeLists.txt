cmake_minimum_required(VERSION 3.2)
project(common_resources)

set(HEADER_FILES
    shared_utils.hpp

    board/Board.hpp
    board/Cell.hpp
    board/Piece.hpp
    board/board_utils.hpp

    messages/message_utils.hpp
    messages/MessageReactor.hpp
    messages/MessageWrapper.hpp
    messages/CheckConnectivity.hpp
    messages/ReplyConnectivity.hpp
    messages/BoardConfigMessage.hpp
    messages/MoveMessage.hpp
    messages/QuitMessage.hpp
    messages/OtherMessage.hpp
    messages/CaseClicked.hpp
)

set(SOURCE_FILES
    shared_utils.cpp

    board/Board.cpp
    board/Cell.cpp
    board/Piece.cpp
    board/board_utils.cpp

    messages/MessageReactor.cpp
    messages/MessageWrapper.cpp
    messages/CheckConnectivity.cpp
    messages/ReplyConnectivity.cpp
    messages/BoardConfigMessage.cpp
    messages/MoveMessage.cpp
    messages/QuitMessage.cpp
    messages/OtherMessage.cpp
    messages/CaseClicked.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ZMQPP
)
