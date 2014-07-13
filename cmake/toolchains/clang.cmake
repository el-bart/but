set(CMAKE_C_COMPILER   clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-Wall -Werror)
add_compile_options(-std=c++1y)
add_compile_options(-fstack-protector-all)

# use LLVM's libc++ instead of GNU's libstdc++
#add_compile_options(-stdlib=libc++)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
