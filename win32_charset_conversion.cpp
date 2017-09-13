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
	int utf8_size = WideCharToMultiByte(
		CP_UTF8, 0, wstr.c_str(), wstr.length(),
		nullptr, 0, nullptr, nullptr);

	if (utf8_size == 0)
	{
		throw win32_error_exception(GetLastError(), L"Converting to unicode failed: ");
	}

	LPSTR c = new char[utf8_size];

	if (WideCharToMultiByte(
		CP_UTF8, 0, wstr.c_str(), wstr.length(),
		c, utf8_size, nullptr, nullptr) != utf8_size)
	{
		delete[] c;
		throw win32_error_exception(GetLastError(), L"Converting to unicode failed: ");
	}

	string str(c, utf8_size);
	delete[] c;

	return str;
}
