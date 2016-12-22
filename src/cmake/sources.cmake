macro(expand_names variable ending)
  foreach(d ${ARGN})
    file(GLOB tmp_srcs ${d}/*.${ending})
    list(APPEND ${variable} ${tmp_srcs})
  endforeach()
endmacro()

set(SRC_DIRS
    But
    But/Log
    But/Log/detail
    But/System
    But/Pattern
    But/Pattern/detail
    But/Threading
)

expand_names(SOURCES_ALL    "cpp"        ${SRC_DIRS})
expand_names(SOURCES_UT     "ut.cpp"     ${SRC_DIRS})
expand_names(SOURCES_MT     "mt.cpp"     ${SRC_DIRS})
expand_names(SOURCES_IT     "it.cpp"     ${SRC_DIRS})
expand_names(SOURCES_MANUAL "manual.cpp" ${SRC_DIRS})

set(SOURCES_LIB ${SOURCES_ALL})
list(REMOVE_ITEM SOURCES_LIB ${SOURCES_UT} ${SOURCES_MT} ${SOURCES_IT} ${SOURCES_MANUAL})