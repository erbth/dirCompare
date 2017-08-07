#ifndef _TOKEN_H
#define _TOKEN_H

#include <memory>
#include <string>

using namespace std;

class Token : public enable_shared_from_this<Token>
{
protected:
	string str;
	Token(string s);

public:
	/* trick to make this class polymorphic */
	virtual string getStr() const;
	void setStr(string s);
};

#endif /* _TOKEN_H */
