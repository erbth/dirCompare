#include <string>
#include "File.h"
#include "Directory.h"
#include "LinuxFile.h"
#include "LinuxDirectory.h"
#include "LinuxItemFactory.h"

File *LinuxItemFactory::createFile(const string& path)
{
	return new LinuxFile(path);
}

Directory *LinuxItemFactory::createDirectory(const string& path)
{
	return new LinuxDirectory(path);
}
