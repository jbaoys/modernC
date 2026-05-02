cmake_minimum_required(VERSION 3.10)
project(Constexpr)
project(Constexpr VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
configure_file(ConstexprConfig.h.in ConstexprConfig.h)
add_executable(Constexpr constexpr.cpp)
target_include_directories(Constexpr PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           HDLC/
                           )
