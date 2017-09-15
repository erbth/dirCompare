#ifndef _WIN32_CHARSET_CONVERSION
#define _WIN32_CHARSET_CONVERSION

#include <string>

std::string wstring_to_string(const std::wstring& wstr);
std::wstring string_to_wstring(const std::string& str);

#endif /* _WIN32_CHARSET_CONVERSION */
