cmake_minimum_required(VERSION 3.10)

project(QuaternionVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

include_directories(
    src
    src/modules/core
    src/modules/graphics
    src/modules/math
)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(quaternion_visualizer
    src/main.cpp
    src/modules/core/Application.cpp
    src/modules/graphics/Window.cpp
    src/modules/graphics/Camera.cpp
    src/modules/graphics/Renderer.cpp
    src/modules/graphics/ObjLoader.cpp
)

target_link_libraries(quaternion_visualizer
    PRIVATE
    SDL2::SDL2main  
    SDL2::SDL2
    SDL2_ttf
)

target_include_directories(quaternion_visualizer
    PRIVATE
    ${SDL2_TTF_INCLUDE_DIRS}
)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/models")

if(EXISTS "${CMAKE_SOURCE_DIR}/models")
    file(COPY "${CMAKE_SOURCE_DIR}/models/" DESTINATION "${CMAKE_BINARY_DIR}/models/")
endif()

# file(GLOB_RECURSE SOURCES "src/*.cpp")

