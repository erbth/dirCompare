#ifndef _KEYWORD_H
#define _KEYWORD_H

#include <memory>
#include "Token.h"

using namespace std;

class Value;

class Keyword :	public Token
{
private:
	shared_ptr<Value> value;
	Keyword(string s);

public:
	static shared_ptr<Keyword> create(string s);

	shared_ptr<Value> getValue() const;
	void setValue(shared_ptr<Value> v);
};

#endif /* _KEYWORD_H */
