add_library(gerbex_svg OBJECT
	SvgSerializer.cpp
)

target_include_directories(gerbex_svg
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(gerbex_svg
PUBLIC
	gerbex_graphics
    pugixml
)
