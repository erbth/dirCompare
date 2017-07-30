#ifndef _ERRNO_EXCEPTION_H
#define _ERRNO_EXCEPTION_H

#include <exception>

#include <errno.h>
#include <string.h>

using namespace std;

class errno_exception : public exception
{
private:
	const char *msg;

public:
	errno_exception(int en)
	{
		msg = strerror(en);
	}

	virtual const char* what() const throw() override
	{
		return msg;
	}
};

#endif /* _ERRNO_EXCEPTION_H */
