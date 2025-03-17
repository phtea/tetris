cmake_minimum_required(VERSION 3.10)
project(Tetris)

set(CMAKE_CXX_STANDARD 17)

# Define a CMake option for DEBUG, default is OFF (disabled)
option(DEBUG "Enable debug mode" OFF)
if(DEBUG)
    message(STATUS "Debug mode ENABLED")
    add_definitions(-DDEBUG=1)
endif()

# Enable debug information for MSVC
if(MSVC)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "EditAndContinue")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

# Set SDL paths
set(SDL3_DIR "C:/lib/SDL3")
set(SDL3_IMAGE_DIR "C:/lib/SDL3_image")
set(SDL3_TTF_DIR "C:/lib/SDL3_ttf")

include_directories(${SDL3_DIR}/include ${SDL3_IMAGE_DIR}/include ${SDL3_TTF_DIR}/include)
link_directories(${SDL3_DIR}/lib/x64 ${SDL3_IMAGE_DIR}/lib/x64 ${SDL3_TTF_DIR}/lib/x64)

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
    src/hud.h src/hud.cpp
    src/custom_types.h
    src/HudBuilder.h src/HudBuilder.cpp)

# Link prebuilt SDL3 and SDL3_image
target_link_libraries(tetris SDL3 SDL3_image SDL3_ttf)

# Copy DLLs after build
add_custom_command(TARGET tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL3_DIR}/lib/x64/SDL3.dll ${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.dll ${SDL3_TTF_DIR}/lib/x64/SDL3_ttf.dll $<TARGET_FILE_DIR:tetris>
    COMMAND_EXPAND_LISTS
)