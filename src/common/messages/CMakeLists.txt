cmake_minimum_required(VERSION 3.2)

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
