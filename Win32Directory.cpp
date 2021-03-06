/** This file is part of dirCompare
 *
 * Copyright 2020 Thomas Erbesdobler <t.erbesdobler@team103.com>
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

#include "Win32Directory.h"
#include "gp_exception.h"
#include "win32_error_exception.h"
#include "win32_charset_conversion.h"
#include "platform.h"
#include "win32_charset_conversion.h"
#include "win32_templates.h"
#include <iostream>
#include <algorithm>

extern "C"
{
#include <Windows.h>
#include <AclAPI.h>
#include <AccCtrl.h>
}

using namespace std;

Win32Directory::Win32Directory(const wstring& wpath, shared_ptr<SystemParameters> sp) :
	Directory(sp)
{
	int pos = wpath.rfind(L'\\');
	if (pos == wstring::npos)
	{
		/* It is a directory name */
		this->wname = wpath;
		this->wpath = wpath;
	}
	else
	{
		if (wpath.size() > 2 && wpath.substr(wpath.size() - 2, 2) == L":\\")
		{
			/* It is a root directory */
			this->wname = L"\\";
			this->wpath = wpath;
		}
		else
		{
			/* It is a path */
			this->wname = wpath.substr(pos + 1LL, wpath.size() - pos - 1);
			this->wpath =  wpath;
		}
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
};

Win32Directory::Win32Directory(const wstring& wname, shared_ptr<SystemParameters> sp, shared_ptr<const Directory> dir) :
	Directory(sp, dir)
{
	this->wname = wname;

	auto p = string_to_wstring(dir->getPath());
	if (p.size() == 0 || *p.crbegin() != L'\\')
		p.push_back(L'\\');

	this->wpath = p + this->wname;

	this->name = wstring_to_string(this->wname);
	this->path = wstring_to_string(this->wpath);

	init();
};

void Win32Directory::init()
{
	// wcout << L"Directory name: \"" << wname << "\", path: \"" << wpath << L"\"" << endl;

	handle = CreateFileW(
		wpath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		nullptr);

	if (handle == INVALID_HANDLE_VALUE)
	{
		throw win32_error_exception(GetLastError());
	}

	if (!(getAttributes() & FILE_ATTRIBUTE_DIRECTORY))
	{
		if (!CloseHandle(handle))
		{
			throw win32_error_exception(
				GetLastError(),
				L"CloseHandle failed, "
				L"closing because file is not a directory: ");
		}

		handle = INVALID_HANDLE_VALUE;

		throw gp_exception("is not a directory");
	}
}

Win32Directory::~Win32Directory()
{
	if (handle != INVALID_HANDLE_VALUE)
	{
		if (!CloseHandle(handle))
		{
			*(sp->getLog()) << endl << "CRITICAL but can't terminate: ~Win32Directory: CloseHandle failed" << endl;
		}
	}

	if (securityInfoValid)
	{
		LocalFree(pSecurityDescriptor);
	}
}

vector<shared_ptr<Item>> Win32Directory::getItems() const
{
	vector<shared_ptr<Item>> v;

	WIN32_FIND_DATAW fd;

	wstring dir_path;

	/* "Prepending the string "\\?\" does not allow access to the root directory." - https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findfirstfilew */
	if (wpath.size() >= 2 && wpath.substr(wpath.size() - 2, 2) == L":\\")
		dir_path = wpath + L"*";
	else
		dir_path = L"\\\\?\\" + wpath + L"\\*";	

	// wcout << L"dir search pattern: \"" << dir_path << "\"" << endl;

	HANDLE fh = FindFirstFileW(dir_path.c_str(), & fd);
	if (fh == INVALID_HANDLE_VALUE)
		throw win32_error_exception(GetLastError(), L"Failed to list files in directory \"" + wpath + L"\": ");

	auto factory = createItemFactory(sp);

	for (;;)
	{
		wstring name(fd.cFileName);

		if (name != L"." && name != L"..")
		{
			/* The factory will create an invalid file / directory as required. It's
			 * safe to not care for that here. */
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				v.push_back(factory->createDirectory(wstring_to_string(name), shared_from_this()));
			else
				v.push_back(factory->createFile(wstring_to_string(name), shared_from_this()));
		}

		if (!FindNextFileW(fh, &fd))
		{
			auto e = GetLastError();
			FindClose(fh);

			if (e == ERROR_NO_MORE_FILES)
				break;
			else
				throw win32_error_exception(e, L"Failed to list files in directory \"" + wpath + L"\": ");
		}
	}

	sort(v.begin(), v.end(), [](shared_ptr<Item> i1, shared_ptr<Item> i2) {
		string p1 = i1->getName();
		string p2 = i2->getName();

		if (p1.compare(p2) < 0)
		{
			return true;
		}
		else
		{
			return false;
		}
		});

	return v;
}

HANDLE Win32Directory::getHandle() const
{
	return handle;
}

void Win32Directory::getFileBasicInfo() const
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

DWORD Win32Directory::getType() const
{
	return 0;
}

uint64_t Win32Directory::getSize() const
{
	return 0;
}

LARGE_INTEGER Win32Directory::getCreationTime() const
{
	getFileBasicInfo();
	return fbi.CreationTime;
}

LARGE_INTEGER Win32Directory::getLastAccessTime() const
{
	getFileBasicInfo();
	return fbi.LastAccessTime;
}

LARGE_INTEGER Win32Directory::getLastWriteTime() const
{
	getFileBasicInfo();
	return fbi.LastWriteTime;
}

LARGE_INTEGER Win32Directory::getChangeTime() const
{
	getFileBasicInfo();
	return fbi.ChangeTime;
}

DWORD Win32Directory::getAttributes() const
{
	getFileBasicInfo();
	return fbi.FileAttributes;
}

void Win32Directory::getSecurityInfo() const
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

const PSID Win32Directory::getOwner() const
{
	getSecurityInfo();
	return pSidOwner;
}

const PSID Win32Directory::getGroup() const
{
	getSecurityInfo();
	return pSidGroup;
}

const PACL Win32Directory::getDacl() const
{
	getSecurityInfo();
	return pDacl;
}

bool Win32Directory::isDaclProtected() const
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

bool Win32Directory::compare_all_attributes_to(const Win32Directory* other, string& reason) const
{
	return compare_all_win32_item_attributes(this, other, reason);
}

