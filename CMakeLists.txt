cmake_minimum_required(VERSION 3.1)

project(ChaosCraze)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)



find_package(Raylib REQUIRED)

add_executable(ChaosCraze main.cpp headers/raylib-tileson.cpp)

target_link_libraries(ChaosCraze PRIVATE -lraylib)
