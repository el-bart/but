add_subdirectory(gmock)

add_executable(but_uts EXCLUDE_FROM_ALL ${SOURCES_UT})
add_executable(but_mts EXCLUDE_FROM_ALL ${SOURCES_MT})
add_executable(but_its EXCLUDE_FROM_ALL ${SOURCES_IT})

foreach(app but_uts but_mts but_its)
  target_link_libraries(${app} but)
  target_link_libraries(${app} gmock_main gmock gtest)
  target_link_libraries(${app} boost_serialization boost_filesystem boost_system)
  add_test(NAME run_${app} COMMAND ${CMAKE_BINARY_DIR}/${app})
  list(APPEND TEST_TARGETS ${app})
endforeach()

foreach(src ${SOURCES_MANUAL})
  get_filename_component(dir  ${src} DIRECTORY)
  get_filename_component(name ${src} NAME_WE)
  set(app but_${name}.manual)
  add_executable(${app} EXCLUDE_FROM_ALL ${src})
  target_link_libraries(${app} but)
  list(APPEND TEST_TARGETS_MANUAL ${app})
  list(APPEND TEST_TARGETS        ${app})
endforeach()

add_custom_target(but_tests_manual DEPENDS ${TEST_TARGETS_MANUAL})
add_custom_target(but_tests_auto   DEPENDS ${TEST_TARGETS})

# enables ctest (otherwise tests would be discarded)
enable_testing()
