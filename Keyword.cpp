#include <string>
#include <memory>
#include "Token.h"
#include "Keyword.h"
#include "Value.h"

Keyword::Keyword(string s) : Token(s)
{
}

shared_ptr<Keyword> Keyword::create(string s)
{
	return shared_ptr<Keyword>(new Keyword(s));
}

shared_ptr<Value> Keyword::getValue() const
{
	return value;
}

void Keyword::setValue(shared_ptr<Value> v)
{
	if (value != v)
	{
		value = v;
		v->setKeyword(static_pointer_cast<Keyword>(shared_from_this()));
	}
}
