cmake_minimum_required(VERSION 3.0)

project(tests)

target_include_directories(ThirdParty INTERFACE ${THIRDPARTY_INCLUDE_DIR})

# Make test executable
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h")
file(GLOB_RECURSE KRAKEN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp")
file(GLOB_RECURSE INIREADER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/third_party/iniReader/*.cpp")

add_executable(tests ${TEST_SOURCES} ${KRAKEN_SOURCES} ${INIREADER_SOURCES})
target_link_libraries(tests ThirdParty)