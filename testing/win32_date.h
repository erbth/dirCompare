#ifndef _WIN32_DATE_H
#define _WIN32_DATE_H

#include <string>

extern "C"
{
	#include <Windows.h>
}

class win32_date
{
protected:
	FILETIME ft;
	SYSTEMTIME st;

	void to_systemtime();

public:
	win32_date(LARGE_INTEGER& l);

	std::wstring format(std::wstring msg = std::wstring());
};

#endif /* _WIN32_DATE_H */
