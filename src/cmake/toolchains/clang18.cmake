SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-18)
set(CMAKE_CXX_COMPILER clang++-18)

set(CMAKE_AR      "llvm-ar-18"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-18"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-18"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-18" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-18"  CACHE FILEPATH "ranlib")
