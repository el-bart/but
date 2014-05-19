add_compile_options(-g3 -ggdb)
if("${SANITIZE}" STREQUAL "address")
  message(STATUS "enabling address sanitizer")
  add_compile_options(-fsanitize=address)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -lasan")
endif()
