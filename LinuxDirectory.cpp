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
