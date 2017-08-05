#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "errno_exception.h"
#include "gp_exception.h"
#include "Item.h"
#include "Directory.h"
#include "LinuxDirectory.h"
#include "LinuxFile.h"
#include "LinuxFileInfo.h"

extern "C"
{
	#include <stdlib.h>
	#include <stddef.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <dirent.h>
	#include <unistd.h>
	#include <errno.h>
}

LinuxDirectory::LinuxDirectory(const string& path) : Directory(path)
{
	init();
}

LinuxDirectory::LinuxDirectory(const string& path, shared_ptr<const Directory> dir)
 : Directory(path, dir)
{
	init();
}

void LinuxDirectory::init()
{
	cout << "Trying to open LinuxDirectory " << path.c_str() << " ..." << endl;

	int fd;

	if (directory)
	{
		fd = openat(
			dynamic_pointer_cast<const LinuxDirectory>(directory)->getFd(),
			path.c_str(),
			O_RDONLY | O_DIRECTORY);
	}
	else
	{
		fd = open(path.c_str(), O_RDONLY | O_DIRECTORY);
	}

	if (fd < 0)
	{
		throw errno_exception(
			errno,
			"failed to open directory " + path + ": ");
	}

	/* create DIR stream */
	dir = fdopendir(fd);

	if (!dir)
	{
		if (close(fd) < 0)
		{
			throw errno_exception(
				errno,
				"failed to close fd, closing triggered by initialization error of "
				+ path + ": ");
		}

		throw errno_exception(
			errno,
			"failed to create DIR stream for " + path + ": ");
	}

	if (!getFileInfo().isDirectory())
	{
		if (closedir(dir) < 0)
		{
			throw errno_exception(
				errno,
				"Failed to close directory " + path + " (initialization error): ");
		}

		throw gp_exception(path + string(" Is not a directory."));
	}

	cout << "LinuxDirectory  " << path << " created" << endl;
}

LinuxDirectory::~LinuxDirectory()
{
	if (closedir(dir) < 0)
	{
		throw errno_exception(errno);
	}

	cout << "LinuxDirectory " << path << " closed" << endl;
}

LinuxFileInfo LinuxDirectory::getFileInfo() const
{
	struct stat st;

	if (fstat(dirfd(dir), &st) < 0)
	{
		throw errno_exception(errno);
	}

	return LinuxFileInfo(&st);
}

vector<shared_ptr<Item>> LinuxDirectory::getItems() const
{
	vector <shared_ptr<Item>> v;

	rewinddir(dir);

	auto name_max = fpathconf(dirfd(dir), _PC_PATH_MAX);

	if (name_max < 0)
	{
		throw errno_exception(
			errno,
			"failed to get maximum allowed file name length: ");
	}

	struct dirent *entry = static_cast<struct dirent*>(
		malloc(offsetof(struct dirent, d_name) + name_max + 1));
	
	if (!entry)
	{
		throw gp_exception("failed to allocate memory for directory entry");
	}

	struct dirent *result;

	for (;;)
	{
		if (readdir_r(dir, entry, &result) < 0)
		{
			free(entry);
			entry = nullptr;
			throw errno_exception(errno, "reading directory " + path + ": ");
		}

		if (result)
		{
			/* exclude special files */
			if (string(result->d_name) == ".." || string(result->d_name) == ".")
			{
				continue;
			}

			if (result->d_type == DT_DIR)
			{
				v.push_back(make_shared<LinuxDirectory>(
					result->d_name,
					shared_from_this()));
			}
			else
			{
				auto f = make_shared<LinuxFile>(
					result->d_name,
					shared_from_this());

				if (result->d_type == DT_UNKNOWN)
				{
					if (f->getFileInfo().isDirectory())
					{
						v.push_back(make_shared<LinuxDirectory>(
							result->d_name,
							shared_from_this()));
					}
				}
				else
				{
					v.push_back(f);
				}
			}
		}
		else
		{
			break;
		}
	}

	free(entry);
	entry = nullptr;

	sort(v.begin(), v.end(),[](shared_ptr<Item> i1, shared_ptr<Item> i2){
		string p1 = i1->getPath();
		string p2 = i2->getPath();

		if (p1.compare(p2) < 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	return v;
}

int LinuxDirectory::getFd() const
{
	return dirfd(dir);
}
