cmake_minimum_required(VERSION 3.0)
project(But)

include(cmake/flags.cmake)
include(cmake/sources.cmake)
include(cmake/coverage.cmake)
include(cmake/sanitizers.cmake)

add_library(but ${SOURCES_LIB})
target_include_directories(but PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(but boost_system)

include(cmake/tests.cmake)
