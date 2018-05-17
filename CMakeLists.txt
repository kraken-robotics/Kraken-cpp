cmake_minimum_required(VERSION 3.0)
project(Kraken)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-DDEBUG=1)

file(GLOB_RECURSE KRAKEN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")

add_executable(Kraken
        main.cpp ${KRAKEN_SOURCES})
include(tests/CMakeLists.txt)
