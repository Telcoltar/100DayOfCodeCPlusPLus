cmake_minimum_required(VERSION 3.17)
project(10_testing)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Exercises exercises.cpp)

target_link_libraries(Exercises gtest gtest_main)