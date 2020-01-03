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
#include "win32_security_tools.h"

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
		// wpath = string_to_wstring(directory->getPath()) + L"\\" + wpath;
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

		buf = new wchar_t[(size_t)len + 1];
		
		if (GetFullPathNameW(wpath.c_str(), len + 1, buf, nullptr) == 0)
		{
			delete[] buf;
			throw win32_error_exception(GetLastError());
		}

		wpath = wstring(buf);
		path = wstring_to_string(wpath);
	}
	
	handle = CreateFileW(
		wstring(L"\\\\?\\" + (directory ? string_to_wstring(directory->getPath()) + L"\\" : L"") + wpath).c_str(),
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
	if (getType() != other->getType())
	{
		reason = "different type";
		return false;
	}

	if (getSize() != other->getSize())
	{
		reason = "different size";
		return false;
	}

	if (getCreationTime().QuadPart != other->getCreationTime().QuadPart)
	{
		reason = "different creation time";
		return false;
	}

	if (getLastAccessTime().QuadPart != other->getLastAccessTime().QuadPart)
	{
		reason = "different last access time";
		return false;
	}

	if (getLastWriteTime().QuadPart != other->getLastWriteTime().QuadPart)
	{
		reason = "different last write time";
		return false;
	}

	if (getChangeTime().QuadPart != other->getChangeTime().QuadPart)
	{
		reason = "different change time";
		return false;
	}

	if (getAttributes() != other->getAttributes())
	{
		reason = "different attributes";
		return false;
	}

	if (!EqualSid(getOwner(), other->getOwner()))
	{
		reason = "different owners";
		return false;
	}

	if (!EqualSid(getGroup(), other->getGroup()))
	{
		reason = "different primary group";
		return false;
	}

	if (isDaclProtected() != other->isDaclProtected())
	{
		reason = "different DACL protection state (ACL inheritance)";
		return false;
	}

	/* Compare DACLs */
	if (getDacl() == nullptr && other->getDacl() != nullptr || getDacl() != nullptr && other->getDacl() == nullptr)
	{
		reason = "only one DACL is a NULL ACL";
		return false;
	}

	PEXPLICIT_ACCESS peACEs1, peACEs2;
	ULONG ceACEs1, ceACEs2;

	auto ret = GetExplicitEntriesFromAcl(getDacl(), &ceACEs1, &peACEs1);
	if (ret != ERROR_SUCCESS)
		throw win32_error_exception(ret, L"GetExplicitEntriesFromAcl failed: ");

	ret = GetExplicitEntriesFromAcl(other->getDacl(), &ceACEs2, &peACEs2);
	if (ret != ERROR_SUCCESS)
	{
		LocalFree(peACEs1);
		throw win32_error_exception(ret, L"GetExplicitEntriesFromAcl failed: ");
	}

	auto different = false;

	if (ceACEs1 != ceACEs2)
	{
		reason = "different count of DACL entries";
		different = true;
	}

	if (!different)
	{
		for (ULONG i = 0; i < ceACEs1; i++)
		{
			bool found = false;

			for (ULONG j = 0; j < ceACEs1; j++)
			{
				if (win32_explicit_aces_equal(peACEs1[i], peACEs2[j]))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				different = true;
				reason = "different DACL entries";
				break;
			}
		}
	}

	if (!different)
	{
		for (ULONG i = 0; i < ceACEs1; i++)
		{
			bool found = false;

			for (ULONG j = 0; j < ceACEs1; j++)
			{
				if (win32_explicit_aces_equal(peACEs2[i], peACEs1[j]))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				different = true;
				reason = "different DACL entries";
				break;
			}
		}
	}

	LocalFree(peACEs2);
	LocalFree(peACEs1);

	if (different)
		return false;

	/* All attributes seem to be equal. */
	return true;
}
