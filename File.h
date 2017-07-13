#include <iostream>
#include <string>

using namespace std;

class File : Item
{
public:
	File(string& path);

	virtual const string& compare(File f);

	virtual ostream& dump(ostream& o) const
	{
		return o << "File: " << path;
	}
};
