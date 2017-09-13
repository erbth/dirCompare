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

#ifndef _WIN32_ERROR_EXCEPTION_H
#define _WIN32_ERROR_EXCEPTION_H

#include <exception>
#include <string>
#include <cstring>

extern "C"
{
	#include <Windows.h>
}

class win32_error_exception : public std::exception
{
private:
	char* msg;

	void init(DWORD en, std::wstring wmsg = std::wstring())
	{
		LPWSTR wstr_msg = nullptr;
		size_t cnt;

		if ((cnt = FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			0,
			en,
			0,
			(LPWSTR) &wstr_msg,
			0,
			nullptr)))
		{
			wmsg += std::wstring(wstr_msg, cnt);
			LocalFree(wstr_msg);
		}
		else
		{
			wmsg += L"Unable to format error message by error " + std::to_wstring(GetLastError());
		}

		auto end = wmsg.cend() - 1;
		while (*end == L'\n' || *end == L'\r' || *end == L' ')
		{
			wmsg.pop_back();
			end = wmsg.cend() - 1;
		}

		int utf8_size = WideCharToMultiByte(
			CP_UTF8, 0, wmsg.c_str(), wmsg.length(),
			nullptr, 0, nullptr, nullptr);

		if (utf8_size == 0)
		{
			special_error();
			return;
		}

		msg = new char[utf8_size + 1];
		msg[utf8_size] = '\0';

		utf8_size = WideCharToMultiByte(
			CP_UTF8, 0, wmsg.c_str(), wmsg.length(),
			msg, utf8_size, nullptr, nullptr);

		if (utf8_size == 0)
		{
			delete[] msg;
			special_error();
			return;
		}
	}

	void special_error()
	{
		std::string s("Failed to convert error message to UTF-8: " + std::to_string(GetLastError()));
		msg = new char[s.length() + 1];
		msg[s.length()] = '\0';
		strcpy(msg, s.c_str());
	}

public:
	win32_error_exception(DWORD en) : msg(nullptr)
	{
		init(en);
	}

	win32_error_exception(DWORD en, std::wstring str) : msg(nullptr)
	{
		init(en, str);
	}

	virtual ~win32_error_exception()
	{
		if (msg)
		{
			delete[] msg;
		}
	}

	bool operator ==(const win32_error_exception& b)
	{
		return strcmp(msg, b.msg) == 0;
	}

	virtual const char* what() const throw() override
	{
		return msg;
	}
};

#endif /* _WIN32_ERROR_EXCEPTION_H */
