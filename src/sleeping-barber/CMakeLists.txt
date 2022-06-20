cmake_minimum_required(VERSION 3.0.0)
project(sleeping-barber VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(sleeping-barber main.cpp)
target_link_libraries(sleeping-barber cpp-terminal)
