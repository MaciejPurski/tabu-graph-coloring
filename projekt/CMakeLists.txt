cmake_minimum_required(VERSION 3.10)

project(tabu-coloring)

set(CMAKE_BUILD_TYPE Debug)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

find_package(Boost 1.65 COMPONENTS graph container REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(tabu-coloring ${SOURCES})
target_link_libraries(tabu-coloring ${Boost_LIBRARIES})




