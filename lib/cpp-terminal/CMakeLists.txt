cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(cpp-terminal
        VERSION 1.0.0
        DESCRIPTION "Small header only C++ library for writing multiplatform terminal applications"
        HOMEPAGE_URL "https://github.com/jupyter-xeus/cpp-terminal"
        LANGUAGES CXX)

# set the C++ standard, if not set by the top level project
if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17
        CACHE STRING "C++ standard" FORCE)
endif ()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(cpp-terminal)
