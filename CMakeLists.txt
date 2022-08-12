cmake_minimum_required (VERSION 2.9)
project (maze_resolve LANGUAGES CXX)

set (SOURCE src/main.cpp src/bmp.cpp src/maze.cpp)

include_directories (src/)
add_executable(maze_resolve ${SOURCE}) 


