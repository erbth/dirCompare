#ifndef _ERRNO_EXCEPTION_H
#define _ERRNO_EXCEPTION_H

#include <exception>

#include <errno.h>
#include <string.h>

using namespace std;

class errno_exception : public exception
{
private:
	const string msg;

public:
	errno_exception(int en) : msg(string(strerror(en)))
	{
	}

	errno_exception(int en, string str) : msg(str + string(strerror(en)))
	{
	}

	virtual const char* what() const throw() override
	{
		return msg.c_str();
	}
};

#endif /* _ERRNO_EXCEPTION_H */
