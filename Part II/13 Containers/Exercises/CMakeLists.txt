cmake_minimum_required(VERSION 3.17)
project(12_utilities)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Exercises exercise_7.cpp)

target_link_libraries(Exercises gtest gtest_main)
