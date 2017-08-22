#include <string>
#include "InvalidItem.h"

using namespace std;

const string InvalidItem::getErrorMessage() const
{
	return errorMessage;
}

void InvalidItem::setErrorMessage(const string& msg)
{
	errorMessage = msg;
}
