cmake_minimum_required(VERSION 3.16)
project(TSPACOFramework CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib QUIET)
if(NOT raylib_FOUND)
    add_subdirectory(third_party/raylib)
endif()

include_directories(src third_party)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)