#ifndef _FILE_H
#define _FILE_H

#include <iostream>
#include <string>
#include "Item.h"

using namespace std;

class File : public Item
{
public:
	File(const string& path);
	File(const char* path);

	virtual ~File();

	virtual ostream& dump(ostream& o) const
	{
		return o << "File: " << path;
	}
};

#endif /* _FILE_H */
