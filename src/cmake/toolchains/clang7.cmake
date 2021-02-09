SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-7)
set(CMAKE_CXX_COMPILER clang++-7)

set(CMAKE_AR      "llvm-ar-7"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-7"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-7"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-7" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-7"  CACHE FILEPATH "ranlib")
