
add_executable(
	testing
	testing.cpp
	$<TARGET_OBJECTS:libDirCompare>
	)

include_directories(testing "${PROJECT_SOURCE_DIR}")
target_link_libraries(testing "${CPPUNIT}")

add_test("CPPUNIT" "${PROJECT_BINARY_DIR}/testing/testing")
