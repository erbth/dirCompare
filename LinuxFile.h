#ifndef _LINUX_FILE_H
#define _LINUX_FILE_H

#include <iostream>
#include <memory>
#include <string>
#include "SystemParameters.h"
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
	LinuxFile(const string& path, shared_ptr<SystemParameters> sp);
	LinuxFile(
		const string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);

	virtual ~LinuxFile();

	virtual LinuxFileInfo getFileInfo() const;

	virtual ostream& dump(ostream& o) const
	{
		return o << "LinuxFile: " << path;
	}
};

#endif /* _LINUX_FILE_H */
