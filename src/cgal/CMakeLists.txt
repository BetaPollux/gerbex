add_library(gerbex_cgal OBJECT
	CgalSerializer.cpp
)

target_include_directories(gerbex_cgal
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Not used, set value to suppress warning
set(CGAL_DATA_DIR ".")
find_package(CGAL REQUIRED
OPTIONAL_COMPONENTS
	Qt5
)
target_link_libraries(gerbex_cgal
PUBLIC
	gerbex_graphics
    CGAL::CGAL
    CGAL::CGAL_Basic_viewer
)
