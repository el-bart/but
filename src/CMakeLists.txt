cmake_minimum_required(VERSION 3.0)
project(But)

include(cmake/flags.cmake)
include(cmake/sources.cmake)
include(cmake/coverage.cmake)

add_library(but ${SOURCES_LIB})
target_include_directories(but PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(but
    boost_filesystem
    boost_system
    pthread
)

include(cmake/tests.cmake)
