#ifndef _LINUX_FACTORY_H
#define _LINUX_FACTORY_H

#include <string>
#include "File.h"
#include "Directory.h"
#include "AbstractFactory.h"

class LinuxFactory : public AbstractFactory
{
public:
	virtual File *createFile(const string& path) override;
	virtual File *createFile(const char* path) override;

	virtual Directory *createDirectory(const string& path) override;
	virtual Directory *createDirectory(const char* path) override;
};

#endif /* _LINUX_FACTORY_H */
