cmake_minimum_required(VERSION 3.16)
project(EcosDelCrepusculo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find raylib
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    # Download raylib
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Include directories
include_directories(src)
include_directories(include)
include_directories(tests)

# Collect source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp" 
    "src/*.c"
    "include/*.h"
    "include/*.hpp"
)

# Collect test files for debug builds
file(GLOB_RECURSE TEST_SOURCES 
    "tests/*.cpp"
    "tests/*.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add test sources in debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_sources(${PROJECT_NAME} PRIVATE ${TEST_SOURCES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_BUILD)
endif()

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Platform specific settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} winmm gdi32 opengl32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32)
    
    # Set subsystem for release builds
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
        )
    endif()
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} pthread m dl rt X11)
endif()

if(APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "-framework OpenGL" 
        "-framework Cocoa" 
        "-framework IOKit" 
        "-framework CoreFoundation"
        "-framework CoreAudio"
        "-framework CoreVideo"
        "-framework AudioToolbox"
    )
endif()

# Compiler specific settings
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _WINSOCK_DEPRECATED_NO_WARNINGS 
        _CRT_SECURE_NO_WARNINGS
    )
    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:__cplusplus)
endif()

# Debug/Release configurations
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

# Graphics API definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE 
    PLATFORM_DESKTOP
    GRAPHICS_API_OPENGL_33
)

# Copy resources to output directory
if(EXISTS "${CMAKE_SOURCE_DIR}/resources")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
endif()