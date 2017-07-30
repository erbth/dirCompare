#ifndef _LINUX_DIRECTORY_H
#define _LINUX_DIRECTORY_H

#include <iostream>
#include <string>
#include <vector>
#include "Directory.h"
#include "Item.h"

class LinuxDirectory : public Directory
{
public:
	LinuxDirectory(const string& path);

	virtual ~LinuxDirectory() override;

	virtual ostream& dump(ostream& o) const
	{
		o << "LinuxDirectory: " << path << "\n" <<
			"-----------------------------------------------" << endl;

		for (auto i : items)
		{
			o << i << endl;
		}

		return o;
	}
};

#endif /* _LINUX_DIRECTORY_H */
