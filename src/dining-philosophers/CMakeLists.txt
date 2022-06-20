cmake_minimum_required(VERSION 3.0.0)
project(dining-philosophers VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(dining-philosophers main.cpp)
target_link_libraries(dining-philosophers cpp-terminal)
