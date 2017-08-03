#ifndef _FILE_H
#define _FILE_H

#include <iostream>
#include <string>
#include "Item.h"
#include "Directory.h"

using namespace std;

class File : public Item
{
public:
	File(const string& path);
	File(const string& path, const Directory* dir);

	virtual ~File();

	virtual ostream& dump(ostream& o) const
	{
		return o << "File: " << path;
	}
};

#endif /* _FILE_H */
