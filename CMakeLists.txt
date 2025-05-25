cmake_minimum_required(VERSION 3.1)
project(BlockSnake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/BlockSnake)

add_executable(BlockSnake src/main.cpp src/Coordinate.cpp src/SnakeList.cpp src/Game.cpp src/Genetic.cpp)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
add_library(genetic_lib src/Genetic.cpp)
add_library(coordinate_lib src/Coordinate.cpp)
add_library(linkedlist_lib include/BlockSnake/LinkedList.cpp)
add_subdirectory(tests)
