cmake_minimum_required(VERSION 3.14)

project(tabu-coloring)

set(CMAKE_BUILD_TYPE Debug)
 
include_directories(include)
 
file(GLOB SOURCES "src/*.cpp")

add_executable(tabu-coloring "src/main.cpp" "src/utils.cpp")


