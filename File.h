#ifndef _FILE_H
#define _FILE_H

#include <iostream>
#include <memory>
#include <string>
#include "SystemParameters.h"
#include "Item.h"
#include "Directory.h"

using namespace std;

class File : public Item
{
public:
	File(const string& path, shared_ptr<SystemParameters> sp);
	File(
		const string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);

	virtual ~File();

	virtual ostream& dump(ostream& o) const override
	{
		return o << "File: " << path;
	}
};

#endif /* _FILE_H */
