SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-19)
set(CMAKE_CXX_COMPILER clang++-19)

set(CMAKE_AR      "llvm-ar-19"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-19"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-19"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-19" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-19"  CACHE FILEPATH "ranlib")
