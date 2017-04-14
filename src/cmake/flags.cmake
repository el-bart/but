set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O3 -g3 -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} -flto")

set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -O3 -g1 -DNDEBUG -pg -flto")
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_PROFILE} -pg -flto")

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


# fix for gmock/gtest issue...
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")

# fix for GCC's performance bug, on inlining heuristics
if(${CMAKE_COMPILER_IS_GNUCXX})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finline-limit=150")
endif()

# strip binaries for GCC
if(${CMAKE_COMPILER_IS_GNUCXX})
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif()
