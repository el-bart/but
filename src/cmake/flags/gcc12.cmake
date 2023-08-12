# there's a bug in GCC 12:
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105562
# if optimizations are enabled incorrect "may be used uninitialized" warnings comming up from standard library's internals.
if("${CMAKE_BUILD_TYPE}" MATCHES "^Release|Relwithdebinfo|Profile$")
  message(WARNING "BUT: disabling -Wmaybe-uninitialized for buggy GCC version")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized")
endif()
