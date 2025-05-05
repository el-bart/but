# catch2
add_executable(but_uts ${SOURCES_UT})
add_executable(but_mts ${SOURCES_MT})
add_executable(but_its ${SOURCES_IT})
foreach(app but_uts but_mts but_its)
  target_link_libraries(${app} but)
  target_link_libraries(${app} Catch2WithMain)
  target_link_libraries(${app} boost_serialization boost_chrono)
  add_test(NAME run_${app}
           COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${app}
                   --order rand
                   --use-colour yes
                   --reporter xml
                   --out "${CMAKE_CURRENT_BINARY_DIR}/${app}_report.xml")
  list(APPEND TEST_TARGETS ${app})
endforeach()

# gtest
add_executable(but_utgs ${SOURCES_UTG})
add_executable(but_mtgs ${SOURCES_MTG})
add_executable(but_itgs ${SOURCES_ITG})
foreach(app but_utgs but_mtgs but_itgs)
  target_link_libraries(${app} but)
  target_link_libraries(${app} gmock_main gmock gtest)
  target_link_libraries(${app} boost_serialization boost_chrono)
  add_test(NAME run_${app}
           COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${app}
                   --gtest_shuffle
                   --gtest_color=yes
                   --gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/${app}_report.xml)
  list(APPEND TEST_TARGETS ${app})
endforeach()

foreach(src ${SOURCES_MANUAL})
  get_filename_component(dir  ${src} DIRECTORY)
  get_filename_component(name ${src} NAME_WE)
  set(app but_${name}.manual)
  add_executable(${app} ${src})
  target_link_libraries(${app} but)
  list(APPEND TEST_TARGETS_MANUAL ${app})
  list(APPEND TEST_TARGETS        ${app})
endforeach()

add_custom_target(but_tests_manual DEPENDS ${TEST_TARGETS_MANUAL})
add_custom_target(but_tests_auto   DEPENDS ${TEST_TARGETS})

# enables ctest (otherwise tests would be discarded)
enable_testing()
