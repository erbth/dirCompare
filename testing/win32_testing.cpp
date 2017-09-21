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

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <memory>
#include <string>
#include <fstream>
#include <exception>
#include "win32_testing.h"
#include "Win32File.h"
#include "win32_error_exception.h"
#include "gp_exception.h"
#include "win32_charset_conversion.h"
#include "StubDirectory.h"

extern "C"
{
	#include <Windows.h>
	#include <Sddl.h>
	#include <Aclapi.h>
	#include <AccCtrl.h>
}

using namespace std;

static bool fsObjectExists(wstring name)
{
	return GetFileAttributesW(name.c_str()) != 0xFFFFFFFF;
}

static wstring getFullPath(wstring str)
{
	LPWSTR buf = nullptr;

	DWORD len = GetFullPathNameW(str.c_str(), 0, nullptr, nullptr);
	if (len == 0)
	{
		throw win32_error_exception(GetLastError());
	}

	buf = new wchar_t[len];

	if (GetFullPathNameW(str.c_str(), len, buf, nullptr) != len - 1)
	{
		throw win32_error_exception(GetLastError());
	}

	return wstring(buf);
}

void Win32FileTest::setUp()
{
	// Create teasting base dir
	testBase = getFullPath(TEMP_TEST_DIR);
	if (!fsObjectExists(testBase))
	{
		if (!CreateDirectoryW((L"\\\\?\\" + testBase).c_str(), nullptr))
		{
			CPPUNIT_FAIL(
				string("Failed to create test directory ") + wstring_to_string(testBase) +
				" with error code " + to_string(GetLastError()));
		}
	}

	// Create test file
	tree90 = testBase + L"\\tree90";
	if (!fsObjectExists(tree90))
	{
		ofstream f(tree90, ios_base::out | ios_base::trunc);
		
		CPPUNIT_ASSERT(f.good());
		f << "tree90 text" << endl;

		f.close();

		// Adjust file properties
		auto hFile = CreateFileW(
			tree90.c_str(),
			FILE_WRITE_ATTRIBUTES | WRITE_OWNER,
			0,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			throw win32_error_exception(GetLastError(),
				L"Unable to open " + tree90 +
				L" for editing attributes and timestamps: ");
		}

		// Scurity info
		PSID pSidAdmin;
		PSID pSidWorld;
		SID_IDENTIFIER_AUTHORITY siaAdmin = SECURITY_NT_AUTHORITY;
		SID_IDENTIFIER_AUTHORITY siaWorld = SECURITY_WORLD_SID_AUTHORITY;

		if (!AllocateAndInitializeSid(
			&siaAdmin,
			2,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0,
			&pSidAdmin))
		{
			throw win32_error_exception(GetLastError(),
				L"AllocateAndInitializeSid (BUILTIN\admin) failed: ");
		}

		if (!AllocateAndInitializeSid(
			&siaWorld,
			1,
			SECURITY_WORLD_RID,
			0, 0, 0, 0, 0, 0, 0,
			&pSidWorld))
		{
			FreeSid(pSidAdmin);
			throw win32_error_exception(GetLastError(),
				L"AllocateAndinitializeSid (world) failed: ");
		}

		try
		{
			setSETakeOwnershipName();
		}
		catch (...)
		{
			FreeSid(pSidWorld);
			FreeSid(pSidAdmin);
			CloseHandle(hFile);
			throw;
		}

		DWORD ret = SetSecurityInfo(
			hFile,
			SE_FILE_OBJECT,
			OWNER_SECURITY_INFORMATION |
			GROUP_SECURITY_INFORMATION,
			pSidAdmin,
			pSidWorld,
			nullptr,
			nullptr);

		FreeSid(pSidWorld);
		FreeSid(pSidAdmin);

		try
		{
			clearSETakeOwnershipName();
		}
		catch (...)
		{
			CloseHandle(hFile);
			throw;
		}

		if (ret != ERROR_SUCCESS)
		{
			CloseHandle(hFile);

			cout << endl << "Error: " << to_string(ret) << endl;

			throw win32_error_exception(ret, L"SetSecurityInfo failed: ");
		}

		// Timestamps, attributes
		FILE_BASIC_INFO fbi = { 0 };

		fbi.CreationTime.QuadPart = 10000000LL;
		fbi.LastAccessTime.QuadPart = 100000000LL;
		fbi.LastWriteTime.QuadPart = 1000000000LL;
		fbi.ChangeTime.QuadPart = 10000000000LL;
		fbi.FileAttributes = FILE_ATTRIBUTE_HIDDEN;

		if (!SetFileInformationByHandle(hFile, FileBasicInfo, &fbi, sizeof fbi))
		{
			CloseHandle(hFile);
			throw win32_error_exception(
				GetLastError(),
				L"Unable to set file basic info of " + tree90 + L": ");
		}

		CloseHandle(hFile);
	}


	// Create test dir with file
	testDir = testBase + L"\\testDir";
	if (!fsObjectExists(testDir))
	{
		if (!CreateDirectoryW((L"\\\\?\\" + testDir).c_str(), nullptr))
		{
			CPPUNIT_FAIL(
				string("Failed to create test directory ") + wstring_to_string(testDir) +
				" with error code " + to_string(GetLastError()));
		}
	}

	// Create file in test dir
	tree98 = testDir + L"\\tree98";
	if (!fsObjectExists(tree98))
	{
		ofstream f(tree98, ios_base::out | ios_base::trunc);

		CPPUNIT_ASSERT(f.good());
		f << "tree98 text" << endl;
	}
}

void Win32FileTest::openValidFileRoot()
{
	auto sp = make_shared<SystemParameters>();
	shared_ptr<Win32File> wf;
	
	try
	{
		wf = make_shared<Win32File>(tree90, sp);
	}
	catch (exception& e)
	{
		CPPUNIT_FAIL(string("Opening " + wstring_to_string(tree90) +
			" file failed: ") + e.what());
		return;
	}
	catch (...)
	{
		CPPUNIT_FAIL("Opening " + wstring_to_string(tree90) +
			" file failed by unspecific reason");
		return;
	}

	CPPUNIT_ASSERT(wf->getPath() == wstring_to_string(tree90));
}

void Win32FileTest::openValidFileSubdir()
{
	auto sp = make_shared<SystemParameters>();
	shared_ptr<Win32File> wf;

	auto sd = make_shared<StubDirectory>(wstring_to_string(testDir), sp);

	try
	{
		wf = make_shared<Win32File>(L"tree98", sp, sd);
	}
	catch (exception& e)
	{
		CPPUNIT_FAIL(string("Opening tree98 file failed: ") + e.what());
		return;
	}
	catch (...)
	{
		CPPUNIT_FAIL("Opening tree98 file failed by unspecific reason");
		return;
	}
}

void Win32FileTest::fileNotFound()
{
	auto sp = make_shared<SystemParameters>();
	shared_ptr<Win32File> wf;

	try
	{
		wf = make_shared<Win32File>(testBase + L"\\asdh", sp);
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

void Win32FileTest::openDirectoryMustFail()
{
	auto sp = make_shared<SystemParameters>();
	shared_ptr<Win32File> wf;

	try
	{
		wf = make_shared<Win32File>(testDir, sp);
	}
	catch (exception& e)
	{
		return;
	}
	catch (...)
	{
		CPPUNIT_FAIL("Opening non-existing file threw an unspecific exception");
		return;
	}

	CPPUNIT_FAIL("Opening non-existing file succeeded");
}

void Win32FileTest::fileProperties()
{
	auto sp = make_shared<SystemParameters>();
	shared_ptr<Win32File> wf;

	try
	{
		wf = make_shared<Win32File>(tree90, sp);
	}
	catch (exception& e)
	{
		CPPUNIT_FAIL(string("Opening " + wstring_to_string(tree90) +
			" file failed: ") + e.what());
		return;
	}
	catch (...)
	{
		CPPUNIT_FAIL("Opening " + wstring_to_string(tree90) +
			" file failed by unspecific reason");
		return;
	}

	// Handle
	CPPUNIT_ASSERT(wf->getHandle() != INVALID_HANDLE_VALUE);

	// Type
	CPPUNIT_ASSERT(wf->getType() == FILE_TYPE_DISK);

	// Size
	CPPUNIT_ASSERT(wf->getSize() == 13);

	// Timestamps
	CPPUNIT_ASSERT(wf->getCreationTime().QuadPart == 10000000LL);
	CPPUNIT_ASSERT(wf->getLastAccessTime().QuadPart == 100000000LL);
	CPPUNIT_ASSERT(wf->getLastWriteTime().QuadPart == 1000000000LL);
	CPPUNIT_ASSERT(wf->getChangeTime().QuadPart == 10000000000LL);

	// Attributes
	CPPUNIT_ASSERT(wf->getAttributes() == FILE_ATTRIBUTE_HIDDEN);

	// Owner
	PSID owner = wf->getOwner();

	PSID pSidAdmin;
	SID_IDENTIFIER_AUTHORITY siaAdmin = SECURITY_NT_AUTHORITY;

	if (!AllocateAndInitializeSid(
		&siaAdmin,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pSidAdmin))
	{
		throw win32_error_exception(GetLastError(),
			L"AllocateAndInitializeSid (BUILTIN\admin) failed: ");
	}

	auto isEqual = EqualSid(owner, pSidAdmin);
	FreeSid(pSidAdmin);

	CPPUNIT_ASSERT(isEqual);

	// Primary group
	PSID group = wf->getGroup();

	PSID pSidWorld;
	SID_IDENTIFIER_AUTHORITY siaWorld = SECURITY_WORLD_SID_AUTHORITY;
	
	if (!AllocateAndInitializeSid(
		&siaWorld,
		1,
		SECURITY_WORLD_RID,
		0, 0, 0, 0, 0, 0, 0,
		&pSidWorld))
	{
		throw win32_error_exception(GetLastError(),
			L"AllocateAndinitializeSid (world) failed: ");
	}

	isEqual = EqualSid(group, pSidWorld);
	FreeSid(pSidWorld);

	CPPUNIT_ASSERT(isEqual);
}

// see https://msdn.microsoft.com/en-us/library/windows/desktop/aa446619(v=vs.85).aspx
static void setPrivilege(
	HANDLE hToken,
	LPCWSTR lpszPrivilege,
	BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValueW(
		nullptr,
		lpszPrivilege,
		&luid))
	{
		throw win32_error_exception(GetLastError(), L"LookupPrivilegeValue failed: ");
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	if (bEnablePrivilege)
	{
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else
	{
		tp.Privileges[0].Attributes = 0;
	}

	// Enable the privilege or disable all privileges
	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof tp,
		nullptr,
		nullptr))
	{
		throw win32_error_exception(GetLastError(), L"AdjustTokenPrivileges failed: ");
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		throw gp_exception("The token does not have the privilege.");
	}
}

// see https://msdn.microsoft.com/en-us/library/windows/desktop/aa379620(v=vs.85).aspx
void setSETakeOwnershipName()
{
	HANDLE hToken;

	// Open handle to access token of calling process
	if (!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES,
		&hToken))
	{
		throw win32_error_exception(GetLastError(), L"OpenProcessToken failed: ");
	}

	// Enable the SE_TAKE_OWNERSHIP_NAME privilege
	setPrivilege(hToken, SE_TAKE_OWNERSHIP_NAME, TRUE);
}

void clearSETakeOwnershipName()
{
	HANDLE hToken;

	// Open handle to access token of calling process
	if (!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES,
		&hToken))
	{
		throw win32_error_exception(GetLastError(), L"OpenProcessToken failed: ");
	}

	// Enable the SE_TAKE_OWNERSHIP_NAME privilege
	setPrivilege(hToken, SE_TAKE_OWNERSHIP_NAME, FALSE);
}
