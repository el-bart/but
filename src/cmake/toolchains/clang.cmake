SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   clang)
set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_AR      "llvm-ar"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "llvm-ld"      CACHE FILEPATH "ld")
set(CMAKE_NM      "llvm-nm"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "llvm-objdump" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "llvm-ranlib"  CACHE FILEPATH "ranlib")
