add_library(gerbex_graphics OBJECT
	Arc.cpp
	ArcSegment.cpp
	BlockAperture.cpp
	Box.cpp
	Circle.cpp
	CircleTemplate.cpp
	Contour.cpp
	DataTypeParser.cpp
	Draw.cpp
	Expression.cpp
	Flash.cpp
	Macro.cpp
	MacroCenterLine.cpp
	MacroCircle.cpp
	MacroOutline.cpp
	MacroPolygon.cpp
	MacroTemplate.cpp
	MacroThermal.cpp
	MacroVectorLine.cpp
	Obround.cpp
	ObroundTemplate.cpp
	Point.cpp
	Polygon.cpp
	PolygonTemplate.cpp
	Rectangle.cpp
	RectangleTemplate.cpp
	Region.cpp
	Segment.cpp
	StepAndRepeat.cpp
	Transform.cpp
)

target_include_directories(gerbex_graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

option(DEBUG_MACRO "print debug information when evaluating macros" OFF)
if (DEBUG_MACRO)
	target_compile_definitions(gerbex_graphics
		PRIVATE 
			DEBUG_MACRO
	)
endif()
