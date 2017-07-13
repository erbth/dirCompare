#include <iostream>
#include <string>
#include <vector>

using namespace std;

class File : Item
{
protected:
	vector<Item> items;

	void addItem(Item& i);

public:
	File(string& path);

	const vector<const Item>& getItems();

	virtual const string& compare(Directory d);

	virtual ostream& dump(ostream& o) const
	{
		return o << "Directory: " << path;
	}
};
