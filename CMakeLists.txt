#
# NOTE: use this file only for development. in bigger project add 'src' subdirectory directly.
#
cmake_minimum_required(VERSION 3.0)
project(But_dev)

add_subdirectory(/usr/src/googletest ${CMAKE_CURRENT_BINARY_DIR}/gtest)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/gtest)

add_subdirectory(src)
