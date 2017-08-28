#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <exception>
#include "platform.h"
#include "SystemParameters.h"
#include "errno_exception.h"
#include "gp_exception.h"
#include "log.h"
#include "Item.h"
#include "Directory.h"
#include "ItemFactory.h"
#include "LinuxItemFactory.h"
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

LinuxDirectory::LinuxDirectory(const string& path, shared_ptr<SystemParameters> sp)
	: Directory(path, sp), dir(nullptr)
{
	init();
}

LinuxDirectory::LinuxDirectory(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: Directory(path, sp, dir), dir(nullptr)
{
	init();
}

void LinuxDirectory::init()
{
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
		throw errno_exception(errno);
	}

	/* create DIR stream */
	dir = fdopendir(fd);

	if (!dir)
	{
		if (close(fd) < 0)
		{
			throw errno_exception(
				errno,
				"failed to close fd, "
				"closing triggered by error in initialization of directory: ");
		}

		throw errno_exception(errno, "failed to create DIR stream: ");
	}

	if (!getFileInfo().isDirectory())
	{
		if (closedir(dir) < 0)
		{
			throw errno_exception(
				errno,
				"Failed to close directory (triggerd by initialization error): ");
		}

		throw gp_exception("Is not a directory.");
	}
}

LinuxDirectory::~LinuxDirectory()
{
	if (dir)
	{
		if (closedir(dir) < 0)
		{
			throw errno_exception(errno);
		}
	}
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
	shared_ptr<ItemFactory> itemFactory = createItemFactory(sp);

	if (dynamic_pointer_cast<LinuxItemFactory>(itemFactory) == nullptr)
	{
		throw gp_exception("Not a createItemFactory has not produced a "
			"LinuxItemFactory in LinuxDirectory");
	}

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
				v.push_back(itemFactory->createDirectory(
					result->d_name,
					shared_from_this()));
			}
			else
			{
				shared_ptr<LinuxFile> f;

				try
				{
					f = static_pointer_cast<LinuxFile>(
						itemFactory->createFile(
							result->d_name,
							shared_from_this()));
				}
				catch (exception& e)
				{
					logIndentation(sp->getLog(), shared_from_this());

					*(sp->getLog()) << "Directory " << path <<
						": Error: " << e.what() << endl;

					continue;
				}

				if (result->d_type == DT_UNKNOWN)
				{
					if (f->getFileInfo().isDirectory())
					{
						v.push_back(itemFactory->createDirectory(
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
