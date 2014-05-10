add_subdirectory(gmock)

add_executable(uts ${SOURCES_UT})
add_executable(mts ${SOURCES_MT})

foreach(app mts uts)
  target_link_libraries(${app} gmock_main gmock gtest)
endforeach()

#add_compile_options(--coverage)
