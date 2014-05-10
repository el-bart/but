file(GLOB SOURCES_GTEST gtest/src/gtest-all.cc)
file(GLOB SOURCES_GMOCK gmock/src/gmock-all.cc)

file(GLOB SOURCES_ALL But/*.cpp)
file(GLOB SOURCES_MT  But/*.mt.cpp)
file(GLOB SOURCES_UT  But/*.ut.cpp)

set(SOURCES_LIB ${SOURCES_ALL})
list(REMOVE_ITEM SOURCES_LIB ${SOURCES_UT} ${SOURCES_MT})
