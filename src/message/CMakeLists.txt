cmake_minimum_required(VERSION 3.2)
project(message_utils)

set(HEADER_FILES
    message_utils.hpp
    MessageReactor.hpp
    MessageWrapper.hpp
    CheckConnectivity.hpp
    ReplyConnectivity.hpp
    BoardConfigMessage.hpp
    MoveMessage.hpp
    QuitMessage.hpp
    OtherMessage.hpp
    CaseClicked.hpp
)

set(SOURCE_FILES
    MessageReactor.cpp
    MessageWrapper.cpp
    CheckConnectivity.cpp
    ReplyConnectivity.cpp
    BoardConfigMessage.cpp
    MoveMessage.cpp
    QuitMessage.cpp
    OtherMessage.cpp
    CaseClicked.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ZMQPP
)
