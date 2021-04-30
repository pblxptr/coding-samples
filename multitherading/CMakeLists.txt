cmake_minimum_required(VERSION 3.12)
project(threads)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIR})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(threads ${FILE}.cpp)

target_link_libraries(threads pthread fmt)
