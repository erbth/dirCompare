#ifndef _LINUX_ITEM_FACTORY_H
#define _LINUX_ITEM_FACTORY_H

#include <string>
#include "File.h"
#include "Directory.h"
#include "ItemFactory.h"

class LinuxItemFactory : public ItemFactory
{
public:
	virtual File *createFile(const string& path) override;
	virtual Directory *createDirectory(const string& path) override;
};

#endif /* _LINUX_ITEM_FACTORY_H */
