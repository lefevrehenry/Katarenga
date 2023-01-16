project(katarenga_server)

set(HEADER_FILES
    src/server/server.hpp
    src/server/server_utils.hpp

    src/server/sockets/connection_socket.hpp
)

set(SOURCE_FILES
    src/server/main.cpp
    src/server/server.cpp
    src/server/server_utils.cpp

    src/server/sockets/connection_socket.cpp
)

# Threads dependency
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ZMQPP
    docopt
    katarenga_common
    Threads::Threads
)

