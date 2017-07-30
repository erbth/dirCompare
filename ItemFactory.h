#ifndef _ITEM_FACTORY_H
#define _ITEM_FACTORY_H

#include <string>
#include "File.h"
#include "Directory.h"

class ItemFactory
{
public:
	virtual File *createFile(const string& path) = 0;
	virtual Directory *createDirectory(const string& path) = 0;
};

#endif /* _ITEM_FACTORY_H */
