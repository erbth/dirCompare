/** This file is part of dirCompare
 *
 * Copyright 2017 Thomas Erbesdobler <t.erbesdobler@team103.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>
#include <string>
#include "SystemParameters.h"
#include "LinuxFile.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "errno_exception.h"
#include "LinuxFileInfo.h"
#include "LinuxDirectory.h"

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

LinuxFile::LinuxFile(const string& path, shared_ptr<SystemParameters> sp)
	: File(path, sp), file(nullptr)
{
	init();
}

LinuxFile::LinuxFile(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: File(path, sp, dir), file(nullptr)
{
	init();
}

void LinuxFile::init()
{
	int fd;

	if (directory)
	{
		fd = openat(
			dynamic_pointer_cast<const LinuxDirectory>(directory)->getFd(),
			path.c_str(),
			O_RDONLY);
	}
	else
	{
		fd = open(path.c_str(), O_RDONLY);
	}

	if (fd < 0)
	{
		throw errno_exception(errno);
	}
		
	/* create FILE stream */
	file = fdopen(fd, "r");

	if (!file)
	{
		if (close(fd) < 0)
		{
			throw errno_exception(
				errno,
				"failed to close fd, "
				"closing triggered by error in initialization of file: ");
		}

		throw errno_exception(errno, "failed to create FILE stream: ");
	}
}

LinuxFile::~LinuxFile()
{
	if (file)
	{
		if (fclose(file) < 0)
		{
			throw errno_exception(errno);
		}
	}
}

LinuxFileInfo LinuxFile::getFileInfo() const
{
	struct stat st;

	if (fstat(fileno(file), &st) < 0)
	{
		throw errno_exception(errno);
	}

	return LinuxFileInfo(&st);
}
