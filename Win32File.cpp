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
#include <cstring>
#include "SystemParameters.h"
#include "Win32File.h"
#include "Item.h"
#include "File.h"
#include "gp_exception.h"
#include "Directory.h"
#include "win32_error_exception.h"
#include "win32_charset_conversion.h"

#include <iostream>

extern "C"
{
	#include <Windows.h>
	#include <AclAPI.h>
	#include <AccCtrl.h>
}

using namespace std;

Win32File::Win32File(const wstring& wpath, shared_ptr<SystemParameters> sp)
	: File(string(), sp), wpath(wpath)
{
	init();
}

Win32File::Win32File(
	const wstring& wpath,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: File(string(), sp, dir), wpath(wpath)
{
	init();
}

void Win32File::init()
{
	if (directory)
	{
		wpath = string_to_wstring(directory->getPath()) + L"\\" + wpath;
		path = wstring_to_string(wpath);
	}
	else
	{
		LPWSTR buf = nullptr;

		DWORD len = GetFullPathNameW(wpath.c_str(), 0, nullptr, nullptr);
		if (len == 0)
		{
			throw win32_error_exception(GetLastError());
		}

		buf = new wchar_t[len];
		
		if (GetFullPathNameW(wpath.c_str(), len, buf, nullptr) != len - 1)
		{
			throw win32_error_exception(GetLastError());
		}

		wpath = wstring(buf);
		path = wstring_to_string(wpath);
	}
	
	handle = CreateFileW(
		wstring(L"\\\\?\\" + wpath).c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr);

	if (handle == INVALID_HANDLE_VALUE)
	{
		throw win32_error_exception(GetLastError());
	}

	if (getAttributes() & FILE_ATTRIBUTE_DIRECTORY)
	{
		if (!CloseHandle(handle))
		{
			throw win32_error_exception(
				GetLastError(),
				L"CloseHandle failed, "
				L"closing because file is a directory: ");
		}

		handle = INVALID_HANDLE_VALUE;

		throw gp_exception("is a directory");
	}
}

Win32File::~Win32File()
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (!CloseHandle(handle))
		{
			*(sp->getLog()) << endl << "CRITICAL but can't terminate: ~WindowsFile: CloseHandle failed" << endl;
		}
	}

	if (securityInfoValid)
	{
		LocalFree(pSecurityDescriptor);
	}
}

HANDLE Win32File::getHandle() const
{
	return handle;
}

DWORD Win32File::getType()
{
	if (fileType == FILE_TYPE_UNKNOWN)
	{
		fileType = GetFileType(handle);

		if (fileType == FILE_TYPE_UNKNOWN)
		{
			auto err = GetLastError();

			if (err != NO_ERROR)
			{
				throw win32_error_exception(err, L"Retrieving type: ");
			}
		}
	}

	return fileType;
}

uint64_t Win32File::getSize()
{
	if (!sizeValid)
	{
		DWORD hSize;
		DWORD lSize = GetFileSize(handle, &hSize);

		if (lSize == INVALID_FILE_SIZE)
		{
			auto err = GetLastError();

			if (err != NO_ERROR)
			{
				throw win32_error_exception(err, L"Retrieving size: ");
			}
		}

		size = ((uint64_t)lSize) | (((uint64_t)hSize) << 32) & 0xffffffff00000000;
		sizeValid = true;
	}
	return size;
}

void Win32File::getFileBasicInfo()
{
	if (!fbiValid)
	{
		memset(&fbi, 0, sizeof fbi);

		if (!GetFileInformationByHandleEx(handle, FileBasicInfo, &fbi, sizeof fbi))
		{
			throw win32_error_exception(GetLastError(), L"Retrieving timestamps: ");
		}
	}
}

LARGE_INTEGER Win32File::getCreationTime()
{
	getFileBasicInfo();
	return fbi.CreationTime;
}

LARGE_INTEGER Win32File::getLastAccessTime()
{
	getFileBasicInfo();
	return fbi.LastAccessTime;
}

LARGE_INTEGER Win32File::getLastWriteTime()
{
	getFileBasicInfo();
	return fbi.LastWriteTime;
}

LARGE_INTEGER Win32File::getChangeTime()
{
	getFileBasicInfo();
	return fbi.ChangeTime;
}

DWORD Win32File::getAttributes()
{
	getFileBasicInfo();
	return fbi.FileAttributes;
}

void Win32File::getSecurityInfo()
{
	if (!securityInfoValid)
	{
		auto ret = GetSecurityInfo(
			handle,
			SE_FILE_OBJECT,
			OWNER_SECURITY_INFORMATION |
			GROUP_SECURITY_INFORMATION |
			DACL_SECURITY_INFORMATION,
			&pSidOwner,
			&pSidGroup,
			&pDacl,
			&pSacl,
			&pSecurityDescriptor);

		pSacl = nullptr;

		if (ret != ERROR_SUCCESS)
		{
			throw win32_error_exception(ret, L"GetSecurityInfo failed: ");
		}

		securityInfoValid = true;
	}
}

const PSID Win32File::getOwner()
{
	getSecurityInfo();
	return pSidOwner;
}

const PSID Win32File::getGroup()
{
	getSecurityInfo();
	return pSidGroup;
}

const PACL Win32File::getDacl()
{
	getSecurityInfo();
	return pDacl;
}

const PACL Win32File::getSacl()
{
	getSecurityInfo();
	return pSacl;
}
