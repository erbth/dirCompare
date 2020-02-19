/** This file is part of dirCompare
 *
 * Copyright 2017-2020 Thomas Erbesdobler <t.erbesdobler@team103.com>
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
#include "win32_templates.h"

#include <iostream>

extern "C"
{
	#include <Windows.h>
	#include <AclAPI.h>
	#include <AccCtrl.h>
}

using namespace std;

Win32File::Win32File(const wstring& wpath, shared_ptr<SystemParameters> sp)
	: File(sp)
{
	int pos = wpath.rfind(L'\\');
	if (pos == wstring::npos)
	{
		/* It is a file name */
		this->wname = wpath;
		this->wpath = wpath;
	}
	else
	{
		/* It is a path */
		this->wname = wpath.substr(pos + 1LL, wpath.size() - pos - 1);
		this->wpath =  wpath;
	}

	LPWSTR buf = nullptr;

	DWORD len = GetFullPathNameW(this->wpath.c_str(), 0, nullptr, nullptr);
	if (len == 0)
	{
		throw win32_error_exception(GetLastError());
	}

	buf = new wchar_t[(size_t)len + 1];
		
	if (GetFullPathNameW(this->wpath.c_str(), len + 1, buf, nullptr) == 0)
	{
		delete[] buf;
		throw win32_error_exception(GetLastError());
	}

	this->wpath = wstring(buf);

	this->name = wstring_to_string(this->wname);
	this->path = wstring_to_string(this->wpath);

	init();
}

Win32File::Win32File(
	const wstring& wname,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: File(sp, dir)
{
	this->wname = wname;
	this->wpath = string_to_wstring(dir->getPath()) + L"\\" + this->wname;

	this->name = wstring_to_string(this->wname);
	this->path = wstring_to_string(this->wpath);

	init();
}

void Win32File::init()
{
	// wcout << L"File name: \"" << wname << "\", path: \"" << wpath << L"\"" << endl;
	
	handle = CreateFileW(
		wpath.c_str(),
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
			*(sp->getLog()) << endl << "CRITICAL but can't terminate: ~Win32File: CloseHandle failed" << endl;
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

DWORD Win32File::getType() const
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

uint64_t Win32File::getSize() const
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

void Win32File::getFileBasicInfo() const
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

LARGE_INTEGER Win32File::getCreationTime() const
{
	getFileBasicInfo();
	return fbi.CreationTime;
}

LARGE_INTEGER Win32File::getLastAccessTime() const
{
	getFileBasicInfo();
	return fbi.LastAccessTime;
}

LARGE_INTEGER Win32File::getLastWriteTime() const
{
	getFileBasicInfo();
	return fbi.LastWriteTime;
}

LARGE_INTEGER Win32File::getChangeTime() const
{
	getFileBasicInfo();
	return fbi.ChangeTime;
}

DWORD Win32File::getAttributes() const
{
	getFileBasicInfo();
	return fbi.FileAttributes;
}

void Win32File::getSecurityInfo() const
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
			nullptr,
			&pSecurityDescriptor);

		if (ret != ERROR_SUCCESS)
		{
			throw win32_error_exception(ret, L"GetSecurityInfo failed: ");
		}

		securityInfoValid = true;
	}
}

const PSID Win32File::getOwner() const
{
	getSecurityInfo();
	return pSidOwner;
}

const PSID Win32File::getGroup() const
{
	getSecurityInfo();
	return pSidGroup;
}

const PACL Win32File::getDacl() const
{
	getSecurityInfo();
	return pDacl;
}

bool Win32File::isDaclProtected() const
{
	getSecurityInfo();

	SECURITY_DESCRIPTOR_CONTROL sdc;
	DWORD rev;

	if (!GetSecurityDescriptorControl(pSecurityDescriptor, &sdc, &rev))
	{
		throw win32_error_exception(
			GetLastError(),
			L"GetSecurityDescriptorControl (DACL) failed: ");
	}

	return sdc & SE_DACL_PROTECTED;
}

bool Win32File::compare_all_attributes_to(const Win32File* other, string& reason) const
{
	return compare_all_win32_item_attributes(this, other, reason);
}
