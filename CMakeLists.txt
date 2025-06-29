cmake_minimum_required(VERSION 3.10)
project(Tetris)

set(CMAKE_CXX_STANDARD 17)

# Optional debug flag
option(DEBUG "Enable debug mode" OFF)
if(DEBUG)
    message(STATUS "Debug mode ENABLED")
    add_definitions(-DDEBUG=1)
endif()

# Project source files
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add executable
add_executable(tetris ${MY_SOURCES})

# Include headers
target_include_directories(tetris PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Platform-specific setup
if(WIN32)
    # Use prebuilt libs under ./libs on Windows
    set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3")
    set(SDL3_IMAGE_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3_image")
    set(SDL3_TTF_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3_ttf")

    include_directories(
	${SDL3_DIR}/include
	${SDL3_IMAGE_DIR}/include
	${SDL3_TTF_DIR}/include
    )

    link_directories(
	${SDL3_DIR}/lib/x64
	${SDL3_IMAGE_DIR}/lib/x64
	${SDL3_TTF_DIR}/lib/x64
    )

    target_link_libraries(tetris
	"${SDL3_DIR}/lib/x64/SDL3.lib"
	"${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.lib"
	"${SDL3_TTF_DIR}/lib/x64/SDL3_ttf.lib"
    )

    # Copy DLLs after build
    add_custom_command(TARGET tetris POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${SDL3_DIR}/lib/x64/SDL3.dll
	${SDL3_IMAGE_DIR}/lib/x64/SDL3_image.dll
	${SDL3_TTF_DIR}/lib/x64/SDL3_ttf.dll
	$<TARGET_FILE_DIR:tetris>
	COMMAND_EXPAND_LISTS
    )

elseif(UNIX)
    # Use user-local install of SDL3 on Linux
    list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")
    find_package(SDL3 REQUIRED CONFIG)
    find_package(SDL3_image REQUIRED CONFIG)
    find_package(SDL3_ttf REQUIRED CONFIG)

    if(NOT SDL3_FOUND OR NOT SDL3_image_FOUND OR NOT SDL3_ttf_FOUND)
	message(FATAL_ERROR "\nSDL3 libraries not found.\n\
	Please install SDL3, SDL3_image, and SDL3_ttf in your ~/.local directory.\n\
	See https://github.com/libsdl-org/SDL for build instructions.\n")
    endif()

    target_link_libraries(tetris
	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_ttf::SDL3_ttf
    )
endif()

# Compile-time resource path definition
target_compile_definitions(tetris
    PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resourses/")
