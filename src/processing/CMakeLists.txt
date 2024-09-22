add_library(gerbex_processing OBJECT
	CommandHandler.cpp
	CommandsProcessor.cpp
	CoordinateData.cpp
	CoordinateFormat.cpp
	DataTypeParser.cpp
	FileParser.cpp
	FileProcessor.cpp
	GraphicsState.cpp
)

target_include_directories(gerbex_processing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(gerbex_processing
PUBLIC
	gerbex_graphics
)
