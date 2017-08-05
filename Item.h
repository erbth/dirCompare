#ifndef _ITEM_H
#define _ITEM_H

#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Directory;

class Item
{
protected:
	string path;
	shared_ptr<const Directory> directory;

public:
	Item(const string& path);
	Item(const string& path, shared_ptr<const Directory> dir);

	virtual ~Item();

	const string& getPath();

	virtual ostream& dump(ostream& o) const
	{
		return o << "Item: " << path;
	}
};

ostream& operator<<(ostream& o, const Item *i);

#endif /* _ITEM_H */
