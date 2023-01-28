project(katarenga)

set(HEADER_FILES
    src/katarenga/client.hpp
    src/katarenga/client_utils.hpp
    src/katarenga/game.hpp

    src/katarenga/sockets/connection_socket.hpp
    src/katarenga/sockets/server_socket.hpp
)

set(SOURCE_FILES
    src/katarenga/main.cpp

    src/katarenga/client.cpp
    src/katarenga/client_utils.cpp
    src/katarenga/game.cpp

    src/katarenga/sockets/connection_socket.cpp
    src/katarenga/sockets/server_socket.cpp
)

## Threads dependency
find_package(Threads REQUIRED)

## GLTK dependency
#find_package(GLTK MODULE QUIET)
#if(NOT GLTK_FOUND)
#    find_package(GLTK CONFIG REQUIRED)
#endif()
#get_target_property(_GLTK_DIR GLTK INTERFACE_INCLUDE_DIRECTORIES)
#message("-- Found GLTK: ${_GLTK_DIR}")

#file(GLOB SOURCE_FILES
#    "*.cpp"
#    "*.hpp"
#)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    katarenga_common
    Threads::Threads
    ZMQPP
#    docopt
#    GLTK
)
