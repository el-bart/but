SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-6.0)
set(CMAKE_CXX_COMPILER clang++-6.0)

set(CMAKE_AR      "llvm-ar-6.0"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-6.0"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-6.0"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-6.0" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-6.0"  CACHE FILEPATH "ranlib")
