add_subdirectory(graphics)
add_subdirectory(processing)

add_library(libgerbex)
target_link_libraries(libgerbex
PUBLIC
	gerbex_graphics
	gerbex_processing
)