cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project (gerbex LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")

# Default to Release build if not specified
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type set to ${CMAKE_BUILD_TYPE}")

# Set folder structure in build folder
include(GNUInstallDirs)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
	${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY
	${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY
	${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

# Offer the user the choice of overriding the installation directories
set (INSTALL_LIBDIR
	${CMAKE_INSTALL_LIBDIR}
	CACHE PATH "Installation directory for libraries")
set (INSTALL_BINDIR
	${CMAKE_INSTALL_BINDIR}
	CACHE PATH "Installation directory for executables")
set (INSTALL_INCLUDEDIR
	${CMAKE_INSTALL_INCLUDEDIR}
	CACHE PATH "Installation directory for header files")

set (DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
set (INSTALL_CMAKEDIR
	${DEF_INSTALL_CMAKEDIR}
	CACHE PATH "Installaton directory for CMake files")

# Report to user
foreach(p LIB BIN INCLUDE CMAKE)
	file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR}
	_path )
	message(STATUS "Installing ${p} components to ${_path}")
	unset (_path)
endforeach()

# Define variables

set(REQUIRED_OPTIONS -Wall)
set(EXTRA_WARNINGS -Wextra -Wpedantic)

add_compile_options(
    ${REQUIRED_OPTIONS}
    ${EXTRA_WARNINGS}
)

# Project targets and sources
add_subdirectory(src)

# Test targets and sources

enable_testing()
add_subdirectory(test)
