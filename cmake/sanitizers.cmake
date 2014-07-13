if(SANITIZE)
  message(STATUS "enabling ${SANITIZE} sanitizer")
  add_compile_options(-fsanitize=${SANITIZE})
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${SANITIZE}")
endif()

if("${SANITIZE}" STREQUAL "address")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lasan")
endif()
