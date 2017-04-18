#
# NOTE: use this file only for development. in bigger project add 'src' subdirectory directly.
#
cmake_minimum_required(VERSION 3.0)
project(But_dev)

add_subdirectory(/usr/src/googletest ${CMAKE_CURRENT_BINARY_DIR}/gtest)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/gtest)
enable_testing()

if(SANITIZE)
  message(STATUS "enabling ${SANITIZE} sanitizer")
  add_compile_options(-fsanitize=${SANITIZE} -fno-sanitize-recover=all)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${SANITIZE}")
endif()

add_subdirectory(src)
