add_library(test_cgal OBJECT
	test_CgalSerializer.cpp
)

target_link_libraries(test_cgal
PUBLIC
	libgerbex
	CppUTest
	CppUTestExt
)

add_executable(cgal_demo
	cgal_demo.cpp
)
target_link_libraries(cgal_demo
PUBLIC
    CGAL::CGAL
    CGAL::CGAL_Basic_viewer
)
