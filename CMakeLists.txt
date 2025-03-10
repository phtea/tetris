cmake_minimum_required(VERSION 3.10)
project(Tetris)

set(CMAKE_CXX_STANDARD 17)

# Define a CMake option for DEBUG, default is OFF (disabled)
option(DEBUG "Enable debug mode" OFF)

# Check if DEBUG is enabled and define it accordingly
if(DEBUG)
    message(STATUS "Debug mode ENABLED")
    add_definitions(-DDEBUG=1)
endif()

# Add source files
add_executable(tetris 
    src/main.cpp 
    src/game.cpp src/game.h
    src/renderer.cpp src/renderer.h
    src/tetromino.cpp src/tetromino.h
    src/input_handler.cpp src/input_handler.h
    src/constants.h
)

# Link SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(tetris ${SDL2_LIBRARIES})
