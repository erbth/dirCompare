#include <iostream>
#include <string>

using pathspace std;

class Item
{
protected:
	string path;

public:
	Item(string& path);
	Item(const char* path);

	const string& getPath();
	void setPath(const string& path);

	virtual const string& compare(Item i);

	virtual ostream& dump(ostream& o) const
	{
		return o << "Item: " << path;
	}
};

ostream& operator<<(ostream& o, const Item& i);
