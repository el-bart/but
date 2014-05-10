add_library(gtest ${SOURCES_GTEST})
add_library(gmock ${SOURCES_GMOCK})

add_executable(uts ${SOURCES_UT})
add_executable(mts ${SOURCES_MT})
