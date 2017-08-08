#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"
#include "LinuxFile.h"
#include "LinuxDirectory.h"
#include "LinuxItemFactory.h"

LinuxItemFactory::LinuxItemFactory(shared_ptr<SystemParameters> sp)
	: ItemFactory(sp)
{
}

shared_ptr<File> LinuxItemFactory::createFile(const string& path)
{
	return make_shared<LinuxFile>(path, sp);
}

shared_ptr<Directory> LinuxItemFactory::createDirectory(const string& path)
{
	return make_shared<LinuxDirectory>(path, sp);
}
