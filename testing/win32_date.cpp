#include <string>
#include "win32_date.h"
#include "win32_error_exception.h"

extern "C"
{
	#include <Windows.h>
}

using namespace std;

win32_date::win32_date(LARGE_INTEGER& l)
{
	ft.dwLowDateTime = l.LowPart;
	ft.dwHighDateTime = l.HighPart;

	to_systemtime();
}

void win32_date::to_systemtime()
{
	if (!FileTimeToSystemTime(&ft, &st))
	{
		throw win32_error_exception(GetLastError(), L"FileTimeToSystemTime failed: ");
	}
}

wstring win32_date::format(wstring msg)
{
	int cchBuf = GetDateFormatEx(
		LOCALE_NAME_USER_DEFAULT,
		DATE_LONGDATE,
		&st,
		NULL,
		NULL,
		0,
		NULL
	);

	if (cchBuf != 0)
	{
		LPWSTR buf = (LPWSTR) GlobalAlloc(0, cchBuf * sizeof(wchar_t));
		if (buf != NULL)
		{
			if (GetDateFormatEx(
				LOCALE_NAME_USER_DEFAULT,
				DATE_LONGDATE,
				&st,
				NULL,
				buf,
				cchBuf,
				NULL
			) != 0)
			{
				return L"%s%s, %02d:%02d:%02d.%04d\n", msg + buf + L", " +
					to_wstring(st.wHour) + L":" + to_wstring(st.wMinute) + L":" +
					to_wstring(st.wSecond) + L"." + to_wstring(st.wMilliseconds);
			}
			else
			{
				throw win32_error_exception(GetLastError(), L"GetDateFormatEx #2 failed: ");
			}
			GlobalFree(buf);
		}
		else
		{
			throw win32_error_exception(GetLastError(), L"LocalAlloc failed: ");
		}
	}
	else
	{
		throw win32_error_exception(GetLastError(), L"GetDateFormatEx #1 failed: ");
	}
}
