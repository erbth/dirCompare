#include <string>
#include "File.h"
#include "Directory.h"
#include "LinuxFile.h"
#include "LinuxDirectory.h"
#include "LinuxFactory.h"

File *LinuxFactory::createFile(const string& path)
{
	return new LinuxFile(path);
}

File *LinuxFactory::createFile(const char* path)
{
	return new LinuxFile(path);
}

Directory *LinuxFactory::createDirectory(const string& path)
{
	return new LinuxDirectory(path);
}

Directory *LinuxFactory::createDirectory(const char* path)
{
	return new LinuxDirectory(path);
}
