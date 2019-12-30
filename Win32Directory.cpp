#include "Win32Directory.h"
#include "gp_exception.h"
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

Win32Directory::Win32Directory(const wstring& path, shared_ptr<SystemParameters> sp) :
	Directory(string(), sp), wpath(path)
{
	init();
};

Win32Directory::Win32Directory(const wstring& path, shared_ptr<SystemParameters> sp, shared_ptr<const Directory> dir) :
	Directory(string(), sp, dir), wpath(path)
{
	init();
};

void Win32Directory::init()
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

		buf = new wchar_t[len + 1];

		if (GetFullPathNameW(wpath.c_str(), len + 1, buf, nullptr) == 0)
		{
			delete[] buf;
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
