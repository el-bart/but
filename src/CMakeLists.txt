cmake_minimum_required(VERSION 3.0)
project(But)

include(cmake/flags.cmake)
include(cmake/sources.cmake)
include(cmake/coverage.cmake)
include(cmake/sanitizers.cmake)

include_directories(BEFORE SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
add_library(but ${SOURCES_LIB})

include(cmake/tests.cmake)
