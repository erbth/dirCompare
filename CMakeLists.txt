# This file is part of dirCompare
#
# Copyright 2017 Thomas Erbesdobler <t.erbesdobler@team103.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 2.8.8)
project(dirCompare)

# The version number
set(DIR_COMPARE_VERSION_MAJOR 0)
set(DIR_COMPARE_VERSION_MINOR 1)

# Compiler flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	set(CMAKE_CXX_FLAGS "-std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "-gdwarf-2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	set(CMAKE_CXX_FLAGS "/EHsc /Za")
	set(CMAKE_CXX_FLAGS_DEBUG "/Od")
else ()
	MESSAGE(WARNING "C++ Compiler ${CMAKE_CXX_COMPILER_ID} is not supported, compiler flags are not set")
endif ()

# Modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Options
option(WITH_TESTING "Build tests" OFF)

# Target platform
if (WIN32)
	set(TARGET_PLATFORM "WINDOWS_WIN32")
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

if (${TARGET_PLATFORM} STREQUAL WINDOWS_WIN32)
	set(PLATFORM_SPECIFIC_SOURCES ${PLATFORM_SPECIFIC_SOURCES}
	)
elseif (${TARGET_PLATFORM} STREQUAL LINUX)
	set(PLATFORM_SPECIFIC_SOURCES ${PLATFORM_SPECIFIC_SOURCES}
		LinuxFile.cpp
		LinuxDirectory.cpp
		LinuxItemFactory.cpp
		LinuxFileInfo.cpp
		LinuxFileComparisonStrategy.cpp
		LinuxDirectoryComparisonStrategy.cpp
		LinuxSimpleFileComparison.cpp
		LinuxComparisonFactory.cpp
	)
endif ()

add_library(
	libDirCompare OBJECT
	ItemFactory.cpp
    Item.cpp
	File.cpp
	Directory.cpp
	InvalidItem.cpp
	InvalidFile.cpp
	InvalidDirectory.cpp
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
	${PLATFORM_SPECIFIC_SOURCES}
	)

add_executable(dirCompare main.cpp $<TARGET_OBJECTS:libDirCompare>)

# Testing
if (${WITH_TESTING})
	enable_testing()
	add_subdirectory("${PROJECT_SOURCE_DIR}/testing")
endif ()
