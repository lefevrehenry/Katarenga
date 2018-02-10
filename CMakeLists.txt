CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(Katarenga)

set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES
    src/Board.hpp
    src/Cell.hpp
    src/Piece.hpp
)

set(SOURCE_FILES
    src/main.cpp
    src/Board.cpp
)

set(SHADER_FILES
)

# Required packages
find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Core Gui Widgets)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENGL_LIBRARIES} Qt5::Core Qt5::Gui)

#========= Linking with local Glew =========#
set(GLEW_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/glew)

find_library(GLEW_LIBRARY
    NAME GLEW
    NO_DEFAULT_PATH
    PATHS ${GLEW_PATH}/lib
)

if(GLEW_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "GLFW_LIBRARY library not found !")
else()
    target_include_directories(${PROJECT_NAME} PUBLIC ${GLEW_PATH}/include)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${GLEW_LIBRARY})
endif()

#========= Linking with local GLFW =========#
set(GLFW_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/GLFW)

find_library(GLFW_LIBRARY
    NAME glfw
    NO_DEFAULT_PATH
    PATHS ${GLFW_PATH}/lib
)

if(GLFW_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "GLFW_LIBRARY library not found !")
else()
    target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_PATH}/include)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW_LIBRARY})
endif()

#========= Linking with local Assimp =========#
set(ASSIMP_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/assimp)

find_library(ASSIMP_LIBRARY
    NAME assimp
    NO_DEFAULT_PATH
    PATHS ${ASSIMP_PATH}/lib
)

if(ASSIMP_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "Assimp library not found !")
else()
    target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_PATH}/include)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${ASSIMP_LIBRARY})
endif()
