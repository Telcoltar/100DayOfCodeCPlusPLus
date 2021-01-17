cmake_minimum_required(VERSION 3.17)
project(20_network_programming_with_boots_asio)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_ROOT "boost_1_75_0")

FIND_PACKAGE(Boost REQUIRED)

ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")

IF (Boost_FOUND)
    message(STATUS "It works!")
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

add_executable(Exercises exercise_3.cpp)

