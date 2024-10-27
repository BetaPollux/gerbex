add_library(test_processing OBJECT
	MockCommandsProcessor.cpp
	test_CommandHandler.cpp
	test_CommandsProcessor.cpp
	test_CoordinateData.cpp
	test_CoordinateFormat.cpp
	test_FileParser.cpp
	test_FileProcessor.cpp
	test_GraphicsState.cpp
)

target_link_libraries(test_processing
PUBLIC
	libgerbex
	test_graphics
	CppUTest
	CppUTestExt
)

target_include_directories(test_processing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
