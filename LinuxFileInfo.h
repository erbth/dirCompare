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

#ifndef _LINUX_FILE_INFO_H
#define _LINUX_FILE_INFO_H

#include <ostream>
#include <chrono>
#include "errno_exception.h"

extern "C"
{
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
}

class file_permission
{
public:
	static const unsigned int none = 0;
	static const unsigned int exec = 1;
	static const unsigned int write = 2;
	static const unsigned int read = 4;
};

enum class file_type: unsigned int
{
	socket,
	link,
	regularFile,
	blockDevice,
	directory,
	characterDevice,
	fifo
};

using namespace std;
using namespace std::chrono;

class LinuxFileInfo
{
private:
	struct stat struct_stat;

	const string permToString(unsigned int p) const;
	const string typeAsString() const;

public:
	LinuxFileInfo(struct stat *s);

	/* get ID of device containing file */
	dev_t getDeviceId() const;

	/* get inode number */
	ino_t getInodeNumber() const;

	bool isSocket() const;
	bool isLink() const;
	bool isRegularFile() const;
	bool isBlockDevice() const;
	bool isDirectory() const;
	bool isCharacterDevice() const;
	bool isFifo() const;

	/* accumulated type, assumes any file is only from on type */
	file_type getType() const;

	bool isUIDSet() const;
	bool isGIDSet() const;
	bool isSticky() const;
	unsigned int getOwnerPermissions() const;
	unsigned int getGroupPermissions() const;
	unsigned int getWorldPermissions() const;

	nlink_t getHardLinkCount() const;
	uid_t getUID() const;
	gid_t getGID() const;

	/* device id in case it is a special file */
	dev_t getSpecialFileDeviceId() const;

	/* total size in bytes */
	off_t getSize() const;

	/* preferred block size for I/O transfers */
	blksize_t getBlockSize() const;

	/* number of allocated 512 byte blocks */
	blkcnt_t getBlocks() const;

	system_clock::time_point getA_time() const;
	system_clock::time_point getM_time() const;
	system_clock::time_point getC_time() const;

	void print_summary(ostream &o) const;

	bool equivalent_to(LinuxFileInfo &f, string* reason = nullptr) const;
};

#endif /* _LINUX_FILE_INFO_H */
