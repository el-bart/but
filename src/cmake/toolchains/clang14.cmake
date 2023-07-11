SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-14)
set(CMAKE_CXX_COMPILER clang++-14)

set(CMAKE_AR      "llvm-ar-14"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-14"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-14"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-14" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-14"  CACHE FILEPATH "ranlib")
