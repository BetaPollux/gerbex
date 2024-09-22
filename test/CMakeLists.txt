add_subdirectory(graphics)
add_subdirectory(processing)

add_executable(test_gerbex
	AllTests.cpp
)

target_link_libraries(test_gerbex
	test_graphics
	test_processing
	CppUTest
)

add_test(
	NAME test_gerbex
	COMMAND test_gerbex
)
