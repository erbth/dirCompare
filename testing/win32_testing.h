#ifndef _WIN32_TESTING_H
#define _WIN32_TESTING_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <memory>
#include <string>

// IMPORTANT: DO ALWAYS CHANGE BOTH! - Ugly, I know.
#define TEMP_TEST_DIR "tmp_test_dir"
#define LTEMP_TEST_DIR L"tmp_test_dir"

class Win32FileTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(Win32FileTest);

	CPPUNIT_TEST(openValidFile);
	CPPUNIT_TEST(fileNotFound);

	CPPUNIT_TEST_SUITE_END();

public:
	Win32FileTest();

	void openValidFile();
	void fileNotFound();
};

#endif /* _WIN32_TESTING_H */