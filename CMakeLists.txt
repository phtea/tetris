cmake_minimum_required(VERSION 3.10)
project(Tetris)

set(CMAKE_CXX_STANDARD 17)

# Make sure the runtime library is statically linked
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Release>:Release>")

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

# Set paths to our local dependencies
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3")
set(SDL3_IMAGE_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3_image")
set(SDL3_TTF_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3_ttf")

include_directories(${CMAKE_SOURCE_DIR}/include ${SDL3_DIR}/include ${SDL3_IMAGE_DIR}/include ${SDL3_TTF_DIR}/include)
link_directories(${SDL3_DIR}/lib/x64 ${SDL3_IMAGE_DIR}/lib/x64 ${SDL3_TTF_DIR}/lib/x64)

# Define MY_SOURCES to be a list of all source files for the project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(tetris "${MY_SOURCES}" "include/GameBuilder.h")

# Link prebuilt SDL3 and SDL3_image
target_link_libraries(tetris SDL3 SDL3_image SDL3_ttf)

target_compile_definitions(tetris
    PUBLIC RESOURSES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resourses/")

# target_compile_definitions("${CMAKE_PROJECT_NAME}"
#     PUBLIC RESOURSES="${CMAKE_CURRENT_SOURCE_DIR}/resourses"

# Copy DLLs after build
add_custom_command(TARGET tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${SDL3_DIR}/lib/x64/SDL3.dll ${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.dll ${SDL3_TTF_DIR}/lib/x64/SDL3_ttf.dll $<TARGET_FILE_DIR:tetris>
    COMMAND_EXPAND_LISTS
)