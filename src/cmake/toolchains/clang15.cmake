SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-15)
set(CMAKE_CXX_COMPILER clang++-15)

set(CMAKE_AR      "llvm-ar-15"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-15"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-15"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-15" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-15"  CACHE FILEPATH "ranlib")
