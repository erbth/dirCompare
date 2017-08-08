#ifndef _LINUX_DIRECTORY_H
#define _LINUX_DIRECTORY_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "Directory.h"
#include "Item.h"
#include "LinuxFileInfo.h"

extern "C"
{
	#include <dirent.h>
}

class LinuxDirectory : public Directory
{
private:
	void init();

protected:
	DIR *dir;

public:
	LinuxDirectory(const string& path, shared_ptr<SystemParameters> sp);
	LinuxDirectory(
		const string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);

	virtual ~LinuxDirectory() override;

	virtual vector<shared_ptr<Item>> getItems() const override;

	LinuxFileInfo getFileInfo() const;

	int getFd() const;

	virtual ostream& dump(ostream& o) const
	{
		o << "LinuxDirectory: " << path << "\n" <<
			"-----------------------------------------------" << endl;

		for (auto i : getItems())
		{
			o << i->getPath() << endl;
		}

		return o;
	}
};

#endif /* _LINUX_DIRECTORY_H */
