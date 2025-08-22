SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-17)
set(CMAKE_CXX_COMPILER clang++-17)

set(CMAKE_AR      "llvm-ar-17"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-17"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-17"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-17" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-17"  CACHE FILEPATH "ranlib")
