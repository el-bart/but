cmake_minimum_required(VERSION 2.8)
project(But)

include(cmake/sources.cmake)

add_library(but ${SOURCES_LIB})

include(cmake/tests.cmake)
