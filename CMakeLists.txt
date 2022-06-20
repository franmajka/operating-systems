cmake_minimum_required(VERSION 3.0.0)
project(os VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_subdirectory(lib)
include_directories(src/utils)

add_subdirectory(src/myls)
add_subdirectory(src/dining-philosophers)
add_subdirectory(src/readers-writers)
add_subdirectory(src/sleeping-barber)
