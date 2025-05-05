macro(expand_names variable ending)
  foreach(d ${ARGN})
    file(GLOB tmp_srcs ${d}/*.${ending})
    list(APPEND ${variable} ${tmp_srcs})
  endforeach()
endmacro()

set(SRC_DIRS
    But
    But/detail
    But/Container
    But/Format
    But/Format/detail
    But/Log
    But/Log/detail
    But/Log/Backend
    But/Log/Field
    But/Log/Field/detail
    But/Log/Backend/detail
    But/Log/Localization
    But/Log/Destination
    But/Log/Destination/Common
    But/Mpl
    But/Mpl/detail
    But/System
    But/Pattern
    But/Pattern/detail
    But/Threading
)

expand_names(SOURCES_ALL    "cpp"        ${SRC_DIRS})
expand_names(SOURCES_MANUAL "manual.cpp" ${SRC_DIRS})
# catch2
expand_names(SOURCES_UT     "ut.cpp"     ${SRC_DIRS})
expand_names(SOURCES_MT     "mt.cpp"     ${SRC_DIRS})
expand_names(SOURCES_IT     "it.cpp"     ${SRC_DIRS})
# gtest
expand_names(SOURCES_UTG    "utg.cpp"    ${SRC_DIRS})
expand_names(SOURCES_MTG    "mtg.cpp"    ${SRC_DIRS})
expand_names(SOURCES_ITG    "itg.cpp"    ${SRC_DIRS})

set(SOURCES_LIB ${SOURCES_ALL})
list(REMOVE_ITEM SOURCES_LIB
  ${SOURCES_MANUAL}
  # catch2
  ${SOURCES_UT}
  ${SOURCES_MT}
  ${SOURCES_IT}
  # gtest
  ${SOURCES_UTG}
  ${SOURCES_MTG}
  ${SOURCES_ITG}
)
