cmake_minimum_required(VERSION 3.18)
project(posix-mq)

find_package(fmt)

add_executable(${CMAKE_PROJECT_NAME}-server server.cpp)
add_executable(${CMAKE_PROJECT_NAME}-client client.cpp)

set_property(TARGET ${CMAKE_PROJECT_NAME}-server PROPERTY CXX_STANDARD 17)
set_property(TARGET ${CMAKE_PROJECT_NAME}-client PROPERTY CXX_STANDARD 17)

target_link_libraries(${CMAKE_PROJECT_NAME}-server fmt::fmt rt)
target_link_libraries(${CMAKE_PROJECT_NAME}-client fmt::fmt rt)
