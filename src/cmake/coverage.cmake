if("${COVERAGE}" STREQUAL "yes")
  message(STATUS "BUT: enabing coverage counting")
  add_compile_options(--coverage)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()
