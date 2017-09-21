#ifndef _WIN32_TESTING_H
#define _WIN32_TESTING_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

#include <memory>
#include <string>

extern "C"
{
	#include <Windows.h>
}

#define TEMP_TEST_DIR L"tmp_test_dir"

class Win32FileTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(Win32FileTest);

	CPPUNIT_TEST(openValidFileRoot);
	CPPUNIT_TEST(openValidFileSubdir);
	CPPUNIT_TEST(fileNotFound);
	CPPUNIT_TEST(openDirectoryMustFail);
	CPPUNIT_TEST(fileProperties);

	CPPUNIT_TEST_SUITE_END();

private:
	std::wstring testBase;
	std::wstring tree90;
	std::wstring testDir;
	std::wstring tree98;

	static void printSid(const PSID sid, const std::string text);

public:
	void setUp();

	void openValidFileRoot();
	void openValidFileSubdir();
	void fileNotFound();
	void openDirectoryMustFail();
	void fileProperties();
};

void setSETakeOwnershipName();
void clearSETakeOwnershipName();

#endif /* _WIN32_TESTING_H */
