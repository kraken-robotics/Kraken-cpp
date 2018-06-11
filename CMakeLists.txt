cmake_minimum_required(VERSION 3.0)
project(Kraken)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(THIRDPARTY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/)

add_library(ThirdParty INTERFACE)
target_include_directories(ThirdParty INTERFACE ${INIREADER_INCLUDE_DIR})
add_definitions(-DDEBUG=1)

file(GLOB_RECURSE KRAKEN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")

file(GLOB_RECURSE INIREADER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/third_party/iniReader/*.c"
                                    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/iniReader/*.cpp")

add_executable(Kraken main.cpp ${KRAKEN_SOURCES} ${INIREADER_SOURCES})
include(tests/CMakeLists.txt)
target_link_libraries(Kraken ThirdParty)