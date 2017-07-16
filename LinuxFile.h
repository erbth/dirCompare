#ifndef _LINUX_FILE_H
#define _LINUX_FILE_H

#include <iostream>
#include <string>
#include "File.h"
#include "Item.h"

using namespace std;

class LinuxFile : public File
{
public:
	LinuxFile(const string& path);
	LinuxFile(const char* path);

	virtual ~LinuxFile();

	virtual const string *compare(Item *i) override;

	virtual ostream& dump(ostream& o) const
	{
		return o << "LinuxFile: " << path;
	}
};

#endif /* _LINUX_FILE_H */
