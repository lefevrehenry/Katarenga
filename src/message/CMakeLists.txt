cmake_minimum_required(VERSION 3.2)
project(message_utils)

### ZeroMQ dependency
#find_package(ZMQPP MODULE QUIET)
#if(NOT ZMQPP_FOUND)
#    find_package(ZMQPP CONFIG REQUIRED)
#endif()
#get_target_property(_ZMQPP_DIR ZMQPP INTERFACE_INCLUDE_DIRECTORIES)
#message("-- Found ZMQPP: ${_ZMQPP_DIR}")

set(HEADER_FILES
    message_utils.hpp
    MessageReactor.hpp
    MessageWrapper.hpp
    #CheckConnectivity.hpp
    BoardConfigMessage.hpp
    MoveMessage.hpp
    QuitMessage.hpp
    OtherMessage.hpp
    CaseClicked.hpp
)

set(SOURCE_FILES
    MessageReactor.cpp
    MessageWrapper.cpp
    #CheckConnectivity.cpp
    BoardConfigMessage.cpp
    MoveMessage.cpp
    QuitMessage.cpp
    OtherMessage.cpp
    CaseClicked.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
#target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#target_link_libraries(${PROJECT_NAME} PRIVATE
#    ZMQPP
#)
