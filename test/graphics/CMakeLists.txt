add_library(test_graphics OBJECT
	GraphicsStringFrom.cpp
	test_Arc.cpp
	test_Arc.cpp
	test_ArcSegment.cpp
	test_BlockAperture.cpp
	test_Box.cpp
	test_Circle.cpp
	test_CircleTemplate.cpp
	test_Contour.cpp
	test_DataTypeParser.cpp
	test_Draw.cpp
	test_Expression.cpp
	test_Flash.cpp
	test_Macro.cpp
	test_MacroCenterLine.cpp
	test_MacroCircle.cpp
	test_MacroOutline.cpp
	test_MacroPolygon.cpp
	test_MacroPrimitive.cpp
	test_MacroTemplate.cpp
	test_MacroThermal.cpp
	test_MacroVectorLine.cpp
	test_Obround.cpp
	test_ObroundTemplate.cpp
	test_Point.cpp
	test_Polygon.cpp
	test_PolygonTemplate.cpp
	test_Rectangle.cpp
	test_RectangleTemplate.cpp
	test_Region.cpp
	test_Segment.cpp
	test_StepAndRepeat.cpp
	test_Transform.cpp
)

target_link_libraries(test_graphics
PUBLIC 
	gerbex_graphics
	CppUTest
)

target_include_directories(test_graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

