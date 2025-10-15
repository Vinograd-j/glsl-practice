cmake_minimum_required(VERSION 4.0)
project(glsl_practice)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        sfml
        GIT_REPOSITORY "https://github.com/SFML/SFML.git"
        GIT_TAG "3.0.2"
)

FetchContent_MakeAvailable(sfml)

add_executable(glsl_practice main.cpp)
target_link_libraries(glsl_practice PRIVATE sfml-audio sfml-graphics sfml-system sfml-window)