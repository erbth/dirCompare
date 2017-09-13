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

#include <memory>
#include <string>
#include <fstream>
#include <exception>
#include "win32_testing.h"
#include "Win32File.h"
#include "win32_error_exception.h"

extern "C"
{
	#include <Windows.h>
}

using namespace std;

static bool fsObjectExists(wstring name)
{
	return GetFileAttributesW(name.c_str()) != 0xFFFFFFFF;
}

Win32FileTest::Win32FileTest()
{
	cout << endl << "baum17" << endl;

	if (!fsObjectExists(LTEMP_TEST_DIR))
	{
		if (!CreateDirectoryW(LTEMP_TEST_DIR, nullptr))
		{
			CPPUNIT_FAIL(
				string("Failed to create test directory ") + TEMP_TEST_DIR +
				" with error code " + to_string(GetLastError()));
		}
	}

	// Create test file
	if (!fsObjectExists(wstring(LTEMP_TEST_DIR) + L"\\test"))
	{
		ofstream f(string(TEMP_TEST_DIR) + "\\test", ios_base::out | ios_base::trunc);
		
		CPPUNIT_ASSERT(f.good());
		f << "tree" << endl;
	}

}

void Win32FileTest::openValidFile()
{
	auto sp = make_shared<SystemParameters>();
	shared_ptr<Win32File> wf;
	
	try
	{
		wf = make_shared<Win32File>(wstring(LTEMP_TEST_DIR) + L"\\test", sp);
	}
	catch (exception& e)
	{
		CPPUNIT_FAIL(string("Opening test file failed: ") + e.what());
	}
	catch (...)
	{
		CPPUNIT_FAIL("Opening test file failed by unspecific reason");
	}
}

void Win32FileTest::fileNotFound()
{
	auto sp = make_shared<SystemParameters>();
	shared_ptr<Win32File> wf;

	try
	{
		wf = make_shared<Win32File>(wstring(LTEMP_TEST_DIR) + L"\\asdh", sp);
	}
	catch (exception& e)
	{
		auto we = dynamic_cast<win32_error_exception*>(&e);
		CPPUNIT_ASSERT(we);

		CPPUNIT_ASSERT(*we == win32_error_exception(2));
		return;
	}
	catch (...)
	{
		CPPUNIT_FAIL("Opening non-existing file threw an unspecific exception");
		return;
	}

	CPPUNIT_FAIL("Opening non-existing file succeeded");
}
