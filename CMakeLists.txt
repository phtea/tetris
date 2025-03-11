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

# Enable debug information and edit-and-continue for Visual Studio in Debug mode
if(MSVC)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "EditAndContinue") # Ensures PDB is generated
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")  # Enable Program Database for Edit and Continue
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")  # Generate debug info in linker

    # Ensure correct runtime linkage
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")  # Multi-threaded DLL runtime (Release)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")  # Multi-threaded Debug DLL runtime (Debug)
endif()

# Define paths to SDL3
set(SDL3_DIR "C:/lib/SDL3")  # Change this to the correct path where SDL3 is installed
set(SDL3_INCLUDE_DIR "${SDL3_DIR}/include")
set(SDL3_LIB_DIR "${SDL3_DIR}/lib/x64")

# Add SDL3 include directory
include_directories(${SDL3_INCLUDE_DIR})

# Add source files
add_executable(tetris 
    src/main.cpp 
    src/game.cpp src/game.h
    src/renderer.cpp src/renderer.h
    src/tetromino.cpp src/tetromino.h
    src/input_handler.cpp src/input_handler.h
    src/constants.h
)

# Link SDL3 library
target_link_libraries(tetris "${SDL3_LIB_DIR}/SDL3.lib")

# Copy SDL3.dll to the output directory after build
add_custom_command(TARGET tetris POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${SDL3_LIB_DIR}/SDL3.dll" $<TARGET_FILE_DIR:tetris>)