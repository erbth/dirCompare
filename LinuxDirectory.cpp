#include <string>
#include <vector>
#include "Item.h"
#include "Directory.h"
#include "LinuxDirectory.h"
#include "LinuxFile.h"

LinuxDirectory::LinuxDirectory(const string& path) : Directory(path)
{
}

LinuxDirectory::~LinuxDirectory()
{
	cout << "LinuxDirectory" << endl;
}

const string *LinuxDirectory::compare(Item *i)
{
	return new string("not matching");
}
