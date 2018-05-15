cmake_minimum_required(VERSION 3.0)
project(Kraken)

set(CMAKE_CXX_STANDARD 11)

add_executable(Kraken
        main.cpp sources/struct/Vector2D.cpp sources/struct/Vector2D.h)
