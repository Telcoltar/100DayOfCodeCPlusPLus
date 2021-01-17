cmake_minimum_required(VERSION 3.17)
project(21_writing_applications)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_ROOT "boost_1_75_0")

FIND_PACKAGE(Boost REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})

IF (Boost_FOUND)
    message(STATUS "It works!")
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

add_executable(Exercises exercise_2.cpp)

