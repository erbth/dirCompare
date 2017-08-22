cmake_minimum_required(VERSION 2.8.8)
project(dirCompare)

# The version number
set(DIR_COMPARE_VERSION_MAJOR 0)
set(DIR_COMPARE_VERSION_MINOR 1)

# Compiler flags
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-gdwarf-2")

# Options
option(WITH_TESTING "Build tests" OFF)

# Target platform
if (WIN32)
	set(TARGET_PLATFORM "WIN32")
else ()
	set(TARGET_PLATFORM "LINUX")
endif ()

# Configure a header file for passing some of of the CMake settings
# to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/dirCompareConfig.h.in"
	"${PROJECT_BINARY_DIR}/dirCompareConfig.h"
	)

include_directories("${PROJECT_BINARY_DIR}")

add_library(
	libDirCompare OBJECT
	ItemFactory.cpp
    Item.cpp
	File.cpp
	Directory.cpp
	InvalidItem.cpp
	InvalidFile.cpp
	InvalidDirectory.cpp
	LinuxFile.cpp
	LinuxDirectory.cpp
	LinuxItemFactory.cpp
	LinuxFileInfo.cpp
	LinuxFileComparisonStrategy.cpp
	LinuxDirectoryComparisonStrategy.cpp
	LinuxSimpleFileComparison.cpp
	LinuxComparisonFactory.cpp
	SimpleDirectoryComparison.cpp
	ComparisonContext.cpp
	ComparisonStrategyFactory.cpp
	FileComparisonStrategy.cpp
	DirectoryComparisonStrategy.cpp
	SystemParameters.cpp
	Token.cpp
	Value.cpp
	Keyword.cpp
	Commandline.cpp
	ignoring.cpp
	platform.cpp
	log.cpp
	)

add_executable(dirCompare main.cpp $<TARGET_OBJECTS:libDirCompare>)

# Testing
if (${WITH_TESTING})
	enable_testing()
	find_library(CPPUNIT "cppunit")

	add_subdirectory("${PROJECT_SOURCE_DIR}/testing")
endif ()
