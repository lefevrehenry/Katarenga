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
    BoardConfiguration.hpp
    CheckConnectivity.hpp
    MessageWrapper.hpp
    message_utils.hpp
)

set(SOURCE_FILES
    BoardConfiguration.cpp
    CheckConnectivity.cpp
    MessageWrapper.cpp
)

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#target_link_libraries(${PROJECT_NAME} PRIVATE
#    ZMQPP
#)
