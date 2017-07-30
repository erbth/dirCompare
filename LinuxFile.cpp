#include <string>
#include <exception>
#include "LinuxFile.h"
#include "File.h"
#include "Item.h"
#include "errno_exception.h"
#include "LinuxFileInfo.h"

#include <iostream>

extern "C"
{
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <errno.h>
}

using namespace std;

LinuxFile::LinuxFile(const string& path) : File(path)
{
	cout << "Trying to open LinuxFile " << path.c_str() << " ..." << endl;

	fd = open(path.c_str(), O_RDONLY);

	if (fd < 0)
	{
		throw errno_exception(errno);
	}

	cout << "LinuxFile " << path << " created" << endl;
}

LinuxFile::~LinuxFile()
{
	if (close(fd) < 0)
	{
		throw errno_exception(errno);
	}

	cout << "LinuxFile " << path << " closed" << endl;
}

LinuxFileInfo LinuxFile::getFileInfo() const
{
	struct stat st;

	if (fstat(fd, &st) < 0)
	{
		throw new errno_exception(errno);
	}

	return LinuxFileInfo(&st);
}
