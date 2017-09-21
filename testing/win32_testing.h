/** This file is part of dirCompare
*
* Copyright 2017 Thomas Erbesdobler <t.erbesdobler@team103.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

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
