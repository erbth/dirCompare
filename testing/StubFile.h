#ifndef _STUB_FILE_H
#define _STUB_FILE_H

#include <ostream>
#include <memory>
#include <string>
#include "File.h"
#include "Directory.h"
#include "SystemParameters.h"

class StubFile : public File
{
public:
	StubFile(const string& path, shared_ptr<SystemParameters> sp);
	StubFile(
		const string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);

	virtual ~StubFile();

	virtual ostream& dump(ostream& o) const override
	{
		return o << "StubFile: " << path;
	}
};

#endif /* _STUB_FILE_H */
