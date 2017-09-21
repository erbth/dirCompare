#include <string>
#include "win32_charset_conversion.h"
#include "win32_error_exception.h"

extern "C"
{
	#include <Windows.h>
}

using namespace std;

string wstring_to_string(const wstring& wstr)
{
	if (wstr.length() > 0)
	{
		int utf8_size = WideCharToMultiByte(
			CP_UTF8, 0, wstr.c_str(), wstr.length(),
			nullptr, 0, nullptr, nullptr);

		if (utf8_size == 0)
		{
			throw win32_error_exception(GetLastError(), L"Converting to unicode string failed: ");
		}

		LPSTR c = new char[utf8_size];

		if (WideCharToMultiByte(
			CP_UTF8, 0, wstr.c_str(), wstr.length(),
			c, utf8_size, nullptr, nullptr) != utf8_size)
		{
			delete[] c;
			throw win32_error_exception(GetLastError(), L"Converting to unicode string failed: ");
		}

		string str(c, utf8_size);
		delete[] c;

		return str;
	}
	else
	{
		return string();
	}
}

wstring string_to_wstring(const string& str)
{
	if (str.length() > 0)
	{
		int wide_size = MultiByteToWideChar(
			CP_UTF8, 0, str.c_str(), str.length(),
			nullptr, 0);

		if (wide_size == 0)
		{
			throw win32_error_exception(GetLastError(), L"Converting to wstring failed: ");
		}

		LPWSTR w = new wchar_t[wide_size];

		if (MultiByteToWideChar(
			CP_UTF8, 0, str.c_str(), str.length(),
			w, wide_size) != wide_size)
		{
			delete[] w;
			throw win32_error_exception(GetLastError(), L"Converting to wstring failed: ");
		}

		wstring wstr(w, wide_size);
		delete[] w;

		return wstr;
	}
	else
	{
		return wstring();
	}
}
