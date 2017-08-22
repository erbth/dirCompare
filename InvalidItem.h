#ifndef _INVALID_ITEM_H
#define _INVALID_ITEM_H

#include <string>

class InvalidItem
{
private:
	std::string errorMessage;

public:
	const std::string getErrorMessage() const;
	void setErrorMessage(const std::string& msg);

	virtual const std::string getErrorDescription() const = 0;
};

#endif /* _INVALID_ITEM_H */
