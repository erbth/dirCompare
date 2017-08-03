#ifndef _LINUX_FILE_H
#define _LINUX_FILE_H

#include <iostream>
#include <string>
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "LinuxFileInfo.h"

extern "C"
{
	#include <stdio.h>
}

using namespace std;

class LinuxFile : public File
{
private:
	void init();

protected:
	FILE* file;

public:
	LinuxFile(const string& path);
	LinuxFile(const string& path, const Directory* dir);

	virtual ~LinuxFile();

	LinuxFileInfo getFileInfo() const;

	virtual ostream& dump(ostream& o) const
	{
		return o << "LinuxFile: " << path;
	}
};

#endif /* _LINUX_FILE_H */
