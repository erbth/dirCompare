#ifndef _INVALID_DIRECTORY_H
#define _INVALID_DIRECTORY_H

#include <ostream>
#include <memory>
#include <string>
#include <vector>
#include "SystemParameters.h"
#include "Directory.h"
#include "InvalidItem.h"

class InvalidDirectory : public Directory, public InvalidItem
{
public:
	InvalidDirectory(const std::string& path, shared_ptr<SystemParameters> sp);
	InvalidDirectory(
		const std::string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);
	
	virtual const std::string getErrorDescription() const override;
	virtual vector<shared_ptr<Item>> getItems() const override;
	
	virtual ostream& dump(ostream& o) const override
	{
		return o << "InvalidDirectory: \"" << path << "\", Error: " <<
			getErrorMessage();
	}
};

#endif /* _INVALID_DIRECTORY_H */
