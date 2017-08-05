#ifndef _LINUX_ITEM_FACTORY_H
#define _LINUX_ITEM_FACTORY_H

#include <memory>
#include <string>
#include "File.h"
#include "Directory.h"
#include "ItemFactory.h"

class LinuxItemFactory : public ItemFactory
{
public:
	virtual shared_ptr<File> createFile(const string& path) override;
	virtual shared_ptr<Directory> createDirectory(const string& path) override;
};

#endif /* _LINUX_ITEM_FACTORY_H */
