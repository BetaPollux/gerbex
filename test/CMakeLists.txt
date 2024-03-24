add_executable(test_gerbex
	AllTests.cpp
	test_Aperture.cpp
	test_ApertureTransformation.cpp
	test_Circle.cpp
	test_CommandsProcessor.cpp
	test_CoordinateFormat.cpp
	test_GraphicalObject.cpp
	test_GraphicsState.cpp
	test_Obround.cpp
	test_Point.cpp
	test_Polygon.cpp
	test_Rectangle.cpp
	test_SyntaxParser.cpp
)

target_link_libraries(test_gerbex
	libgerbex
	CppUTest
)

add_test(
	NAME test_gerbex
	COMMAND test_gerbex
)
