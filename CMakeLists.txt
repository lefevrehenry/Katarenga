cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


option(BUILD_SERVER "Whether to build the server executable" OFF)
if (BUILD_SERVER)
    message(STATUS "The server executable will be built")
endif()
option(BUILD_CLIENT "Whether to build the client executable" ON)
if (BUILD_CLIENT)
    message(STATUS "The client executable will be built")
endif()


if (BUILD_SERVER OR BUILD_CLIENT)
    ## ZeroMQ dependency
    find_package(ZMQPP 4.2.0 MODULE QUIET)
    if(NOT ZMQPP_FOUND)
        find_package(ZMQPP CONFIG REQUIRED)
    endif()
    get_target_property(_ZMQPP_DIR ZMQPP INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "Found ZMQPP: ${_ZMQPP_DIR}")

    ## Docopt dependency
    find_package(docopt MODULE QUIET)
    if(NOT DOCOPT_FOUND)
        find_package(docopt CONFIG REQUIRED)
    endif()
    get_target_property(_DOCOPT_DIR docopt INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "Found docopt: ${_DOCOPT_DIR}")

    # build common library
    message(STATUS "Configuring messages build")
    add_subdirectory(src/message)
endif()

# build the server executable
if (BUILD_SERVER)
    message("-- Configuring server build")
    add_subdirectory(src/server)
endif()

# build the katarenga executable
if (BUILD_CLIENT)
    message("-- Configuring client build")
    add_subdirectory(src/katarenga)
endif()
