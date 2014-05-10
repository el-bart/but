add_compile_options(-g3 -ggdb)
add_compile_options(-fsanitize=address)
set(CMAKE_EXE_LINKER_FLAGS -fsanitize=address)
