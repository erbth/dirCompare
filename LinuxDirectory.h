#ifndef _LINUX_DIRECTORY_H
#define _LINUX_DIRECTORY_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
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
	LinuxDirectory(const string& path);
	LinuxDirectory(const string& path, const Directory* dir);

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
