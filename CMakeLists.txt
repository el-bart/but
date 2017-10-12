#
# NOTE: use this file only for development. in bigger project add 'src' subdirectory directly.
#
cmake_minimum_required(VERSION 3.0)
project(But_dev)

if(NOT BUT_DISABLE_TESTS)
  add_subdirectory(/usr/src/googletest ${CMAKE_CURRENT_BINARY_DIR}/gtest)
  include_directories(${CMAKE_CURRENT_BINARY_DIR}/gtest)
  enable_testing()
endif()

# NOTE: LTO is now optional, since it causes issues with GCC-7.1 and sanitizers...
if(BUT_ENABLE_LTO)
  message(STATUS "enabling LTO")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")

  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")

  set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -flto")
  set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_PROFILE} -flto")

  # LTO plugin flags for AR and RANLIB are needed for GCC only
  if(${CMAKE_COMPILER_IS_GNUCXX})
    # unfortunately 'archive' flags do not have per-build-type variants...
    if(DEFINED CMAKE_BUILD_TYPE AND NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      execute_process(COMMAND gcc --print-file-name=liblto_plugin.so
                      OUTPUT_VARIABLE LTO_PLUGIN_PATH
                      OUTPUT_STRIP_TRAILING_WHITESPACE)
      message(STATUS "using LTO plugin path: ${LTO_PLUGIN_PATH}")
      set(LTO_PLUGIN "--plugin=\"${LTO_PLUGIN_PATH}\"")
      set(CMAKE_CXX_ARCHIVE_CREATE "${CMAKE_CXX_ARCHIVE_CREATE} ${LTO_PLUGIN}")
      set(CMAKE_CXX_ARCHIVE_FINISH "${CMAKE_CXX_ARCHIVE_FINISH} ${LTO_PLUGIN}")
    endif()
  endif()
endif()

if(BUT_SANITIZE)
  message(STATUS "enabling ${BUT_SANITIZE} sanitizer")
  add_compile_options(-fsanitize=${BUT_SANITIZE} -fno-sanitize-recover=all)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${BUT_SANITIZE} -fno-sanitize-recover=all")
endif()

add_subdirectory(src)
