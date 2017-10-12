install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/But
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")
install(TARGETS but
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
