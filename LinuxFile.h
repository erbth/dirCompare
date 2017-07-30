#ifndef _LINUX_FILE_H
#define _LINUX_FILE_H

#include <iostream>
#include <string>
#include "File.h"
#include "Item.h"
#include "LinuxFileInfo.h"

using namespace std;

class LinuxFile : public File
{
protected:
	int fd;

public:
	LinuxFile(const string& path);

	virtual ~LinuxFile();

	virtual const string *compare(Item *i) override;

	LinuxFileInfo getFileInfo();

	virtual ostream& dump(ostream& o) const
	{
		return o << "LinuxFile: " << path;
	}
};

#endif /* _LINUX_FILE_H */
