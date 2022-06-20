cmake_minimum_required(VERSION 3.0.0)
project(readers-writers VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(readers-writers main.cpp)
target_link_libraries(readers-writers cpp-terminal)
