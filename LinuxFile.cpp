#include <string>
#include "LinuxFile.h"
#include "File.h"
#include "Item.h"

using namespace std;

LinuxFile::LinuxFile(const string& path) : File(path)
{
}

LinuxFile::LinuxFile(const char* path) : File(path)
{
}

LinuxFile::~LinuxFile()
{
	cout << "File " << path << " deleted." << endl;
}

const string *LinuxFile::compare(Item *i)
{
	return new string("not matching");
}
