#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "Item.h"

using namespace std;

class Directory : public Item, public enable_shared_from_this<Directory>
{
public:
	Directory(const string& path, shared_ptr<SystemParameters> sp);
	Directory(
		const string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);

	virtual ~Directory();

	virtual vector<shared_ptr<Item>> getItems() const = 0;

	virtual ostream& dump(ostream& o) const
	{
		o << "Directory: " << path << "\n" <<
			"-----------------------------------------------" << endl;

		for (auto i : getItems())
		{
			o << i->getPath() << endl;
		}

		return o;
	}
};

#endif /* _DIRECTORY_H */
