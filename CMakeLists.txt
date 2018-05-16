cmake_minimum_required(VERSION 3.0)
project(Kraken)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-DDEBUG=1)


add_executable(Kraken
        main.cpp sources/struct/vector_2d.cpp sources/struct/vector_2d.h)

include(tests/CMakeLists.txt)