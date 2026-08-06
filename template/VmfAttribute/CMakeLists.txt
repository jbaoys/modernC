cmake_minimum_required(VERSION 3.14)
project(VmfAttributeTest)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage --coverage")

#option to build googletest
include(../../buildLocalGtest.cmake)

add_executable(
    ${PROJECT_NAME}
    VmfDebug.cc
    VmfAttributeTest.cc
    VmfAttributeBufferTest.cc
)

target_link_libraries(
    ${PROJECT_NAME}
    gtest
    gtest_main
    gmock
    gmock_main
)
