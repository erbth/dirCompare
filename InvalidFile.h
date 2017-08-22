#ifndef _INVALID_FILE_H
#define _INVALID_FILE_H

#include <ostream>
#include <memory>
#include <string>
#include "File.h"
#include "Directory.h"
#include "InvalidItem.h"

class InvalidFile : public File, public InvalidItem
{
public:
	InvalidFile(const std::string& path, shared_ptr<SystemParameters> sp);
	InvalidFile(
		const std::string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);
	
	virtual const std::string getErrorDescription() const override;

	virtual ostream& dump(ostream& o) const override
	{
		return o << "InvalidFile: \"" << path << "\", Error: " <<
			getErrorMessage();
	}
};

#endif /* _INVALID_FILE_H */
