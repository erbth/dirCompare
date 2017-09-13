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
#include "SystemParameters.h"
#include "Win32File.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "win32_error_exception.h"
#include "win32_charset_conversion.h"

#include <iostream>

extern "C"
{
	#include <Windows.h>
}

using namespace std;

Win32File::Win32File(const wstring& wpath, shared_ptr<SystemParameters> sp)
	: File(string(), sp), handle(INVALID_HANDLE_VALUE), wpath(wpath)
{
	init();
}

Win32File::Win32File(
	const wstring& wpath,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: File(string(), sp, dir), handle(INVALID_HANDLE_VALUE), wpath(wpath)
{
	init();
}

void Win32File::init()
{
	if (directory)
	{
		path += "\\" + directory->getPath();
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
}

HANDLE Win32File::getHandle() const
{
	return handle;
}