#ifndef _ABSTRACT_FACTORY_H
#define _ABSTRACT_FACTORY_H

#include <string>
#include "File.h"
#include "Directory.h"

class AbstractFactory
{
public:
	virtual File *createFile(const string& path) = 0;
	virtual Directory *createDirectory(const string& path) = 0;
};

#endif /* _ABSTRACT_FACTORY_H */
