# based on http://myprogrammingnotesgish.blogspot.de/2014/05/add-cppunit-using-cmake-in-ubuntu.html
#
# - This module finds the CppUnit library
#
# It sets the following variables:
# CPPUNIT_FOUND			- Set to false, or undefined, if CppUnit isn't found.
# CPPUNIT_INCLUDE_DIR	- The CppUnit include directory.
# CPPUNIT_LIBRARY		- The CppUnit library to link against.

find_path(CPPUNIT_INCLUDE_DIR cppunit/Test.h HINTS "${CPPUNIT_ROOT}/include")
find_library(CPPUNIT_LIBRARY NAMES cppunit HINTS "${CPPUNIT_ROOT}/lib")

if (CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY)
	set(CPPUNIT_FOUND TRUE)
else ()
	set(CPPUNIT_FOUND FALSE)
endif ()

if (CPPUNIT_FOUND)
	# show which CppUnit was found only if not quiet
	if (NOT CppUnit_FIND_QUIETLY)
		message(STATUS "Found CppUnit: ${CPPUNIT_LIBRARY}")
	endif ()
else ()
	if (CppUnit_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find CppUnit")
	endif ()
endif ()
