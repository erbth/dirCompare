#include <iostream>
#include <string>

using namespace std;

class Item
{
protected:
	string name;

public:
	Item(string& name);
	Item(const char* name);

	const string& getName();
	void setName(const string& name);

	virtual const string& compare(Item i);

	virtual ostream& dump(ostream& o) const
	{
		return o << "Item: " << name;
	}
};

ostream& operator<<(ostream& o, const Item& i);
