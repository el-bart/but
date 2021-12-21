SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang-9)
set(CMAKE_CXX_COMPILER clang++-9)

set(CMAKE_AR      "llvm-ar-9"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld-9"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm-9"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump-9" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib-9"  CACHE FILEPATH "ranlib")
