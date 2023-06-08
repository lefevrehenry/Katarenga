cmake_minimum_required(VERSION 3.15)
project(katarenga_glfw_render_engine)

set(HEADER_FILES
    BoardEntity.hpp
    BoardRenderEntity.hpp
    PawnEntity.hpp
    PawnRenderEntity.hpp
)

set(SOURCE_FILES
    main.cpp
    BoardEntity.cpp
    BoardRenderEntity.cpp
    PawnEntity.cpp
    PawnRenderEntity.cpp
)

find_package(RenderEngine REQUIRED CONFIG)
find_package(glfw3 3.2.1 REQUIRED)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE katarenga)
target_link_libraries(${PROJECT_NAME} PRIVATE RenderEngine::RenderEngine glfw)
