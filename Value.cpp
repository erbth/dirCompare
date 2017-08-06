#include <memory>
#include <string>
#include "Token.h"
#include "Value.h"
#include "Keyword.h"

Value::Value(string s) : Token(s)
{
}

shared_ptr<Value> Value::create(string s)
{
	return shared_ptr<Value>(new Value(s));
}

shared_ptr<Keyword> Value::getKeyword() const
{
	return keyword;
}

void Value::setKeyword(shared_ptr<Keyword> k)
{
	if (keyword != k)
	{
		keyword = k;
		k->setValue(shared_from_this());
	}
}
