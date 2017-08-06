#include <string>
#include "Token.h"

Token::Token(string s)
{
	str = s;
}

string Token::getStr() const
{
	return str;
}

void Token::setStr(string s)
{
	str = s;
}
