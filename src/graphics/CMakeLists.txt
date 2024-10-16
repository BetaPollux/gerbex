add_library(gerbex_graphics OBJECT
	ApertureTemplate.cpp
	ApertureTransformation.cpp
	Arc.cpp
	ArcSegment.cpp
	BlockAperture.cpp
	Circle.cpp
	CircleTemplate.cpp
	DataTypeParser.cpp
	Draw.cpp
	Expression.cpp
	Flash.cpp
	Macro.cpp
	MacroCenterLine.cpp
	MacroCircle.cpp
	MacroOutline.cpp
	MacroPolygon.cpp
	MacroPrimitive.cpp
	MacroTemplate.cpp
	MacroThermal.cpp
	MacroVectorLine.cpp
	Obround.cpp
	ObroundTemplate.cpp
	Polygon.cpp
	PolygonTemplate.cpp
	Rectangle.cpp
	RectangleTemplate.cpp
	Region.cpp
	RegionContour.cpp
	Segment.cpp
	StepAndRepeat.cpp
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
