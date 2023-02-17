project(katarenga)

add_subdirectory(app)

set(HEADER_FILES
#    src/client/application.hpp
    src/client/client.hpp
    src/client/client_utils.hpp
    src/client/game.hpp

#    src/client/messages/client_messages.hpp

#    src/client/sockets/application_socket.hpp
    src/client/sockets/connection_socket.hpp
    src/client/sockets/server_socket.hpp
#    src/client/sockets/worker_socket.hpp
)

set(SOURCE_FILES
#    src/client/main.cpp

#    src/client/application.cpp
    src/client/client.cpp
    src/client/client_utils.cpp
    src/client/game.cpp

#    src/client/messages/client_messages.cpp

#    src/client/sockets/application_socket.cpp
    src/client/sockets/connection_socket.cpp
    src/client/sockets/server_socket.cpp
#    src/client/sockets/worker_socket.cpp
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

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC katarenga_common
    Threads::Threads
    ZMQPP
#    GLTK
)
