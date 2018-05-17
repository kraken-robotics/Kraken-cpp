cmake_minimum_required(VERSION 3.0)
project(Kraken)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-DDEBUG=1)


add_executable(Kraken
        main.cpp sources/struct/vector_2d.cpp sources/struct/vector_2d.h sources/struct/itinerary_point.cpp sources/struct/itinerary_point.h)

include(tests/CMakeLists.txt)
