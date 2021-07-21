cmake_minimum_required(VERSION 3.18)
project(perfect-forwarding-fail-cases)

find_package(fmt)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(${CMAKE_PROJECT_NAME} fmt::fmt rt)
