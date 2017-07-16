#include <string>
#include <vector>
#include "Item.h"
#include "Directory.h"
#include "LinuxDirectory.h"
#include "LinuxFile.h"

LinuxDirectory::LinuxDirectory(const string& path) : Directory(path)
{
}

LinuxDirectory::LinuxDirectory(const char* path) : Directory(path)
{
	items.push_back(new LinuxFile("test"));
	items.push_back(new LinuxFile("baum"));
}

LinuxDirectory::~LinuxDirectory()
{
	cout << "LinuxDirectory" << endl;
}

const string *LinuxDirectory::compare(Item *i)
{
	return new string("not matching");
}
