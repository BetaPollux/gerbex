add_library(test_processing OBJECT
	MockAperture.cpp
	MockCommandsProcessor.cpp
	MockTemplate.cpp
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
	CppUTest
	CppUTestExt
)
