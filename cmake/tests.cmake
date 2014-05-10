add_subdirectory(gmock)

add_executable(uts ${SOURCES_UT})
add_executable(mts ${SOURCES_MT})

foreach(app mts uts)
  target_link_libraries(${app} gmock_main gmock gtest)
  set_target_properties(${app} PROPERTIES COMPILE_FLAGS --coverage)
  set_target_properties(${app} PROPERTIES LINK_FLAGS    --coverage)
endforeach()
