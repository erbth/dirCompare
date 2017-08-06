#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

using namespace std;

class Token
{
protected:
	string str;
	Token(string s);

public:
	string getStr() const;
	void setStr(string s);
};

#endif /* _TOKEN_H */
