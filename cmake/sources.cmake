file(GLOB SOURCES_ALL
        But/*.cpp
        But/Threading/*.cpp
    )
file(GLOB SOURCES_MT
        But/*.mt.cpp
        But/Threading/*.mt.cpp
    )
file(GLOB SOURCES_UT
        But/*.ut.cpp
        But/Threading/*.ut.cpp
    )

set(SOURCES_LIB ${SOURCES_ALL})
list(REMOVE_ITEM SOURCES_LIB ${SOURCES_UT} ${SOURCES_MT})
