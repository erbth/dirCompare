#ifndef _ITEM_H
#define _ITEM_H

#include <iostream>
#include <string>

using namespace std;

class Item
{
protected:
	string path;

public:
	Item(const string& path);

	virtual ~Item();

	const string& getPath();
	void setPath(const string& path);

	virtual const string *compare(Item *i) = 0;

	virtual ostream& dump(ostream& o) const
	{
		return o << "Item: " << path;
	}
};

ostream& operator<<(ostream& o, const Item *i);

#endif /* _ITEM_H */
