cmake_minimum_required(VERSION 3.0)

include(cmake/defaults.cmake)
include(cmake/modes/${MODE}.cmake)
include(cmake/toolchains/${TOOLCHAIN}.cmake)

project(But)

include(cmake/sources.cmake)
include(cmake/coverage.cmake)
include(cmake/sanitizers.cmake)

include_directories(BEFORE SYSTEM ${CMAKE_SOURCE_DIR})
add_library(but ${SOURCES_LIB})

include(cmake/tests.cmake)
