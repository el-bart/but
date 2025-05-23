SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-16)
set(CMAKE_CXX_COMPILER clang++-16)

set(CMAKE_AR      "llvm-ar-16"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-16"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-16"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-16" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-16"  CACHE FILEPATH "ranlib")
