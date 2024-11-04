add_library(test_cgal OBJECT
	test_CgalSerializer.cpp
)

target_link_libraries(test_cgal
PUBLIC
	libgerbex
	CppUTest
	CppUTestExt
)
