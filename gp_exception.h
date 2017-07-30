#ifndef _GP_EXCEPTION_H
#define _GP_EXCEPTION_H

#include <exception>

#include <string.h>

using namespace std;

class gp_exception : public exception
{
private:
	string msg;

public:
	gp_exception(const string& msg)
	{
		this->msg.assign(msg);
	}

	virtual const char* what() const throw() override
	{
		return msg.c_str();
	}
};

#endif /* _GP_EXCEPTION_H */
