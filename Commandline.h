#ifndef _COMMANDLINE_H
#define _COMMANDLINE_H

#include <memory>
#include <vector>
#include <string>
#include "Token.h"
#include "SystemParameters.h"

using namespace std;

class Commandline
{
private:
	vector<shared_ptr<Token>> tokens;

	void addToken(const string& str);

public:
	Commandline(int argy, char** argv);

	void parse(shared_ptr<SystemParameters> param) const;
};

#endif /* _COMMANDLINE_H */
