#idf_component_register(SRCS "countedString.c" INCLUDE_DIRS "/.")

#set_source_files_properties(countedString.c PROPERTIES COMPILE_FLAGS -fexceptions)

add_library (countedString countedString.c)
target_include_directories (countedString PUBLIC "/")