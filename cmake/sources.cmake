# TODO: make macro for expanding all of these variables using directory list
file(GLOB SOURCES_ALL
        But/*.cpp
        But/Log/*.cpp
        But/Log/detail/*.cpp
        But/Threading/*.cpp
    )
file(GLOB SOURCES_MT
        But/*.mt.cpp
        But/Log/*.mt.cpp
        But/Log/detail/*.mt.cpp
        But/Threading/*.mt.cpp
    )
file(GLOB SOURCES_UT
        But/*.ut.cpp
        But/Log/*.ut.cpp
        But/Log/detail/*.ut.cpp
        But/Threading/*.ut.cpp
    )

set(SOURCES_LIB ${SOURCES_ALL})
list(REMOVE_ITEM SOURCES_LIB ${SOURCES_UT} ${SOURCES_MT})
