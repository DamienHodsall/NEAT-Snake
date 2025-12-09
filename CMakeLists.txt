cmake_minimum_required(VERSION 3.8)
project(NeatSnake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/NeatSnake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Serial-linux)

add_library(Serial-linux include/Serial-linux/Serial.cpp)

add_executable(NeatSnake src/main.cpp src/Game.cpp src/Genetic.cpp)
target_link_libraries(NeatSnake pthread Serial-linux)

add_executable(WatchGame src/watchGame.cpp src/Game.cpp src/Genetic.cpp)
target_link_libraries(WatchGame pthread Serial-linux)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
add_library(genetic_lib src/Genetic.cpp)
add_library(thread_lib src/ThreadPool.cpp)
add_library(game_lib src/Game.cpp)
add_subdirectory(tests)
