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

	virtual shared_ptr<File> createFile(
		const string& path,
		shared_ptr<const Directory> parent = nullptr) noexcept = 0;

	virtual shared_ptr<Directory> createDirectory(
		const string& path,
		shared_ptr<const Directory> parent = nullptr) noexcept = 0;
};

#endif /* _ITEM_FACTORY_H */
