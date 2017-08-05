#include <memory>
#include <string>
#include "File.h"
#include "Directory.h"
#include "LinuxFile.h"
#include "LinuxDirectory.h"
#include "LinuxItemFactory.h"

shared_ptr<File> LinuxItemFactory::createFile(const string& path)
{
	return make_shared<LinuxFile>(path);
}

shared_ptr<Directory> LinuxItemFactory::createDirectory(const string& path)
{
	return make_shared<LinuxDirectory>(path);
}
