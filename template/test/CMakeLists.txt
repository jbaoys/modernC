cmake_minimum_required(VERSION 3.14)
project(myGTestHelloWorld)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage --coverage")

find_package(GTest REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b10fad38c4026a29ea6561ab15fc4818170d1c10.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  MyTest
  MyTest.cc
)
target_link_libraries(
  MyTest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(MyTest)
