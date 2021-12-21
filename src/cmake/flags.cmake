set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

if(${BUT_ENABLE_PIC})
  message(STATUS "BUT: enable PIC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O3 -g3 -DNDEBUG")

set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_PROFILE} -O3 -g1 -DNDEBUG -pg")
set(CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS_PROFILE} -pg")

if(${CMAKE_COMPILER_IS_GNUCXX})
  # enable incremental linking for gcc in debug mode
  #if(DEFINED CMAKE_BUILD_TYPE AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  #    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fno-use-linker-plugin -Wl,--incremental")
  #endif()
endif()

# fix for gmock/gtest issue...
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")

# fix for GCC's performance bug, on inlining heuristics
if(${CMAKE_COMPILER_IS_GNUCXX})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finline-limit=150")
endif()

if(${CMAKE_COMPILER_IS_GNUCXX})
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s") # strip binaries for GCC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-noexcept-type") # remove nonhelpful warning
endif()

# include toolchain-specific flags (if present)
get_filename_component(TC_FILE "${CMAKE_TOOLCHAIN_FILE}" NAME)
include(cmake/flags/${TC_FILE} OPTIONAL)
