cmake_minimum_required(VERSION 3.0)
project(But)

include(cmake/flags.cmake)
include(cmake/sources.cmake)
include(cmake/coverage.cmake)

add_library(but ${SOURCES_LIB})
target_include_directories(but PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(but
    stdc++fs
    boost_system
    pthread
)

if(BUT_ENABLE_TESTS)
  include(cmake/tests.cmake)
endif()
include(cmake/install.cmake)
