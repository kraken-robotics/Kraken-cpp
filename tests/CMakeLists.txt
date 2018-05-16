cmake_minimum_required(VERSION 3.0)

project(tests)

# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h")
file(GLOB_RECURSE KRAKEN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")
add_executable(tests ${TEST_SOURCES} ${KRAKEN_SOURCES} )
target_link_libraries(tests Catch)