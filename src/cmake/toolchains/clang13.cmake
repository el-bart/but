SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-13)
set(CMAKE_CXX_COMPILER clang++-13)

set(CMAKE_AR      "llvm-ar-13"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-13"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-13"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-13" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-13"  CACHE FILEPATH "ranlib")
