add_library(test_svg OBJECT
	test_SvgSerializer.cpp
)

target_link_libraries(test_svg
PUBLIC
	libgerbex
	CppUTest
	CppUTestExt
)
