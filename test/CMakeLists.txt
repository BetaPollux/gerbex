add_executable(test_gerbex
	AllTests.cpp
	test_Arc.cpp
	test_ApertureTransformation.cpp
	test_Arc.cpp
	test_BlockAperture.cpp
	test_Circle.cpp
	test_CircleTemplate.cpp
	test_CommandsProcessor.cpp
	test_CoordinateFormat.cpp
	test_Draw.cpp
	test_Flash.cpp
	test_GraphicsState.cpp
	test_Macro.cpp
	test_MacroPrimitive.cpp
	test_MacroTemplate.cpp
	test_Obround.cpp
	test_ObroundTemplate.cpp
	test_Point.cpp
	test_Polygon.cpp
	test_PolygonTemplate.cpp
	test_Rectangle.cpp
	test_RectangleTemplate.cpp
	test_Region.cpp
	test_RegionContour.cpp
	test_Segment.cpp
	test_StepAndRepeat.cpp
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
