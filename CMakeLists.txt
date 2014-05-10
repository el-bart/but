cmake_minimum_required(VERSION 2.8)

include(cmake/defaults.cmake)
include(cmake/modes/${MODE}.cmake)
include(cmake/toolchains/${TOOLCHAIN}.cmake)
include(cmake/sources.cmake)

project(But)

add_library(but ${SOURCES_LIB})

include(cmake/tests.cmake)
