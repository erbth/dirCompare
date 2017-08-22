#ifndef _LINUX_ITEM_FACTORY_H
#define _LINUX_ITEM_FACTORY_H

#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"
#include "ItemFactory.h"

class LinuxItemFactory : public ItemFactory
{
private:
	template<class T> shared_ptr<T> createInvalidItem(
		const string& path,
		shared_ptr<const Directory> parent,
		const string message) const;

public:
	LinuxItemFactory(shared_ptr<SystemParameters> sp);

	virtual shared_ptr<File> createFile(
		const string& path,
		shared_ptr<const Directory> parent = nullptr) noexcept override;

	virtual shared_ptr<Directory> createDirectory(
		const string& path,
		shared_ptr<const Directory> parent = nullptr) noexcept override;
};

#endif /* _LINUX_ITEM_FACTORY_H */
