#ifndef _STUB_DIRECTORY_H
#define _STUB_DIRECTORY_H

#include <ostream>
#include <memory>
#include <string>
#include <vector>
#include "SystemParameters.h"
#include "Directory.h"
#include "Item.h"

class StubDirectory : public Directory
{
public:
	StubDirectory(const string& path, shared_ptr<SystemParameters> sp);
	StubDirectory(
		const string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);

	virtual ~StubDirectory();

	virtual vector<shared_ptr<Item>> getItems() const override;

	virtual ostream& dump(ostream& o) const
	{
		o << "StubDirectory: " << getPath() << "\n" <<
			"-----------------------------------------------" << endl;

		for (auto i : getItems())
		{
			o << i->getPath() << endl;
		}

		return o;
	}
};

#endif /* _STUB_DIRECTORY_H */
