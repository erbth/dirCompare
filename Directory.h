#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#include <iostream>
#include <string>
#include <vector>
#include "Item.h"

using namespace std;

class Directory : public Item
{
protected:
	vector<Item *> items;

	void addItem(Item& i);

public:
	Directory(const string& path);
	Directory(const char* path);

	virtual ~Directory();

	vector<Item *> *getItems();

	virtual const string *compare(Item *i) = 0;

	virtual ostream& dump(ostream& o) const
	{
		o << "Directory: " << path << "\n" <<
			"-----------------------------------------------" << endl;

		for (auto i : items)
		{
			o << i << endl;
		}

		return o;
	}
};

#endif /* _DIRECTORY_H */
