cmake_minimum_required(VERSION 3.1)

project(ChaosCraze)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# On Windows, adjust the raylib path

# Add include directories

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
        GIT_SHALLOW 1
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Add your executable
add_executable(ChaosCraze main.cpp)

# Link with raylib library
target_link_libraries(ChaosCraze raylib)

# Additional link flags if needed
#target_link_libraries(ChaosCraze -static) # Example for static linking
