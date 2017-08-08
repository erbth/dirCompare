#ifndef _ITEM_FACTORY_H
#define _ITEM_FACTORY_H

#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"

class ItemFactory
{
protected:
	shared_ptr<SystemParameters> sp;

public:
	ItemFactory(shared_ptr<SystemParameters> sp);

	virtual shared_ptr<File> createFile(const string& path) = 0;
	virtual shared_ptr<Directory> createDirectory(const string& path) = 0;
};

#endif /* _ITEM_FACTORY_H */
