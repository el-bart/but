SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-8)
set(CMAKE_CXX_COMPILER clang++-8)

set(CMAKE_AR      "llvm-ar-8"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-8"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-8"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-8" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-8"  CACHE FILEPATH "ranlib")
