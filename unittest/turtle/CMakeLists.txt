# Example for using gmock
# run it with
#   cmake -S. -Bbd
#   cmake --build bd

cmake_minimum_required(VERSION 3.14)
project(turtleTest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set gcovr flags
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage --coverage")

# option to build googletest
# include(../../buildLocalGtest.cmake)

# add executables
add_executable(
    turtleTest
    MockTurtleTest.cc
)

# Add gtest and gmock libraries, note: this requires gtest and gmock be installed
# with make install from google test source build tree.
target_link_libraries(turtleTest gtest gtest_main gmock gmock_main)
