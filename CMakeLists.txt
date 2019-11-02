CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project("Katarenga")

set(CMAKE_BUILD_TYPE RELEASE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")

################
# Dependencies #
################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake/Modules/")

## ZeroMQ dependency
find_package(ZMQ REQUIRED)
include_directories(${ZMQ_INCLUDE_DIRS})

## Docopt dependency
find_package(docopt REQUIRED)
include_directories(${DOCOPT_INCLUDE_DIRS})


## GLTK dependency
set(CMAKE_PREFIX_PATH "/home/mommessc/Documents/Git/OpenGL/build/install/lib/cmake")
find_package(GLTK CONFIG REQUIRED)

file(GLOB katarenga_SRC
    "src/*.cpp"
    "src/*.hpp")

set(SHADER_FILES
)


add_executable(katarenga ${katarenga_SRC} ${SHADER_FILES})

target_link_libraries(katarenga
    ${ZMQ_LIBRARIES}
    ${DOCOPT_LIBRARIES}
	GLTK
)