SET(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER   gcc-9)
set(CMAKE_CXX_COMPILER g++-9)

set(CMAKE_AR      "ar"      CACHE FILEPATH "ar")
set(CMAKE_LINKER  "ld"      CACHE FILEPATH "ld")
set(CMAKE_NM      "nm"      CACHE FILEPATH "nm")
set(CMAKE_OBJDUMP "objdump" CACHE FILEPATH "objdump")
set(CMAKE_RANLIB  "ranlib"  CACHE FILEPATH "ranlib")
