#ifndef _VALUE_H
#define _VALUE_H

#include <memory>
#include "Token.h"

using namespace std;

class Keyword;

class Value :
	public Token,
	public enable_shared_from_this<Value>
{
private:
	shared_ptr<Keyword> keyword;
	Value(string s);

public:
	static shared_ptr<Value> create(string s);

	shared_ptr<Keyword> getKeyword() const;
	void setKeyword(shared_ptr<Keyword> k);
};

#endif /* _VALUE_H */
