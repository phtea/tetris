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

include(FetchContent)

# Fetch SDL3 from GitHub
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.8
)

# Make SDL3 available
FetchContent_MakeAvailable(SDL3)

# Add source files
add_executable(tetris 
    src/main.cpp 
    src/game.cpp src/game.h
    src/renderer.cpp src/renderer.h
    src/tetromino.cpp src/tetromino.h
    src/input_handler.cpp src/input_handler.h
    src/bag7.cpp src/bag7.h
    src/grid.cpp src/grid.h
    src/constants.h
)

# Link SDL3 dynamically
target_link_libraries(tetris SDL3::SDL3)

# Copy SDL3.dll after build
add_custom_command(TARGET tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:tetris> $<TARGET_FILE_DIR:tetris>
    COMMAND_EXPAND_LISTS
)