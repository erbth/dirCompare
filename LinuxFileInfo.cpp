#include <ostream>
#include <chrono>
#include <ctime>
#include "gp_exception.h"
#include "LinuxFileInfo.h"

using namespace std;
using namespace std::chrono;

extern "C"
{
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <unistd.h>
}

LinuxFileInfo::LinuxFileInfo(struct stat *s)
{
	struct_stat = *s;
}

/* get ID of device containing file */
dev_t LinuxFileInfo::getDeviceId() const
{
	return struct_stat.st_dev;
}

/* get inode number */
ino_t LinuxFileInfo::getInodeNumber() const
{
	return struct_stat.st_ino;
}

bool LinuxFileInfo::isSocket() const
{
	return S_ISSOCK(struct_stat.st_mode);
}

bool LinuxFileInfo::isLink() const
{
	return S_ISLNK(struct_stat.st_mode);
}

bool LinuxFileInfo::isRegularFile() const
{
	return S_ISREG(struct_stat.st_mode);
}

bool LinuxFileInfo::isBlockDevice() const
{
	return S_ISBLK(struct_stat.st_mode);
}

bool LinuxFileInfo::isDirectory() const
{
	return S_ISDIR(struct_stat.st_mode);
}

bool LinuxFileInfo::isCharacterDevice() const
{
	return S_ISCHR(struct_stat.st_mode);
}

bool LinuxFileInfo::isFifo() const
{
	return S_ISFIFO(struct_stat.st_mode);
}

/* accumulated type, assumes any file is only from on type */
file_type LinuxFileInfo::getType() const
{
	if (isSocket())
		return file_type::socket;

	if (isLink())
		return file_type::link;

	if (isRegularFile())
		return file_type::regularFile;

	if (isBlockDevice())
		return file_type::blockDevice;
	
	if (isDirectory())
		return file_type::directory;
	
	if (isCharacterDevice())
		return file_type::characterDevice;
	
	if (isFifo())
		return file_type::fifo;
	
	throw gp_exception("unknown file type: no type attribute set");
}

const string LinuxFileInfo::typeAsString() const
{
	if (isSocket())
		return "socket";

	if (isLink())
		return "link";

	if (isRegularFile())
		return "regular file";

	if (isBlockDevice())
		return "block device";
	
	if (isDirectory())
		return "directory";
	
	if (isCharacterDevice())
		return "character device";
	
	if (isFifo())
		return "fifo";
	
	throw gp_exception("unknown file type: no type attribute set");
}



bool LinuxFileInfo::isUIDSet() const
{
	return struct_stat.st_mode & S_ISUID;
}

bool LinuxFileInfo::isGIDSet() const
{
	return struct_stat.st_mode & S_ISGID;
}

bool LinuxFileInfo::isSticky() const
{
	return struct_stat.st_mode & S_ISVTX;
}


unsigned int LinuxFileInfo::getOwnerPermissions() const
{
	unsigned int p = 0;

	if (struct_stat.st_mode & S_IRUSR)
		p |= file_permission::read;

	if (struct_stat.st_mode & S_IWUSR)
		p |= file_permission::write;

	if (struct_stat.st_mode & S_IXUSR)
		p |= file_permission::exec;
	
	return p;
}

unsigned int LinuxFileInfo::getGroupPermissions() const
{
	unsigned int p = 0;

	if (struct_stat.st_mode & S_IRGRP)
		p |= file_permission::read;

	if (struct_stat.st_mode & S_IWGRP)
		p |= file_permission::write;

	if (struct_stat.st_mode & S_IXGRP)
		p |= file_permission::exec;
	
	return p;
}

unsigned int LinuxFileInfo::getWorldPermissions() const
{
	unsigned int p = 0;

	if (struct_stat.st_mode & S_IROTH)
		p |= file_permission::read;

	if (struct_stat.st_mode & S_IWOTH)
		p |= file_permission::write;

	if (struct_stat.st_mode & S_IXOTH)
		p |= file_permission::exec;
	
	return p;
}


nlink_t LinuxFileInfo::getHardLinkCount() const
{
	return struct_stat.st_nlink;
}

uid_t LinuxFileInfo::getUID() const
{
	return struct_stat.st_uid;
}

gid_t LinuxFileInfo::getGID() const
{
	return struct_stat.st_gid;
}


/* device id in case it is a special file */
dev_t LinuxFileInfo::getSpecialFileDeviceId() const
{
	return struct_stat.st_rdev;
}

/* total size in bytes */
off_t LinuxFileInfo::getSize() const
{
	return struct_stat.st_size;
}

/* preferred block size for I/O transfers */
blksize_t LinuxFileInfo::getBlockSize() const
{
	return struct_stat.st_blksize;
}

/* number of allocated 512 byte blocks */
blkcnt_t LinuxFileInfo::getBlocks() const
{
	return struct_stat.st_blocks;
}

system_clock::time_point LinuxFileInfo::getA_time() const
{
	auto d = seconds{struct_stat.st_atim.tv_sec} ;
			 nanoseconds{struct_stat.st_atim.tv_nsec};

	system_clock::time_point tp(duration_cast<system_clock::duration>(d));
	return tp;
}

system_clock::time_point LinuxFileInfo::getM_time() const
{
	auto d = seconds{struct_stat.st_mtim.tv_sec} +
			 nanoseconds{struct_stat.st_mtim.tv_nsec};

	system_clock::time_point tp(duration_cast<system_clock::duration>(d));
	return tp;
}

system_clock::time_point LinuxFileInfo::getC_time() const
{
	auto d = seconds{struct_stat.st_ctim.tv_sec} +
			 nanoseconds{struct_stat.st_ctim.tv_nsec};

	system_clock::time_point tp(duration_cast<system_clock::duration>(d));
	return tp;
}


const string LinuxFileInfo::permToString(unsigned int p) const
{
	string s;
	
	s.push_back(p & file_permission::read ? 'r' : '-');
	s.push_back(p & file_permission::write ? 'w' : '-');
	s.push_back(p & file_permission::exec ? 'x' : '-');

	return s;
}

void LinuxFileInfo::print_summary(ostream& o) const
{
	o << "device ID:    " << getDeviceId() << endl;
	o << "inode number: " << getInodeNumber() << endl;

	o << "type :        " << typeAsString() << endl;

	o << "UID flag:     " << isUIDSet() << endl;
	o << "GID flag:     " << isGIDSet() << endl;
	o << "Sticky flag:  " << isSticky() << endl;

	o << "Owner:        " << permToString(getOwnerPermissions()) << endl;
	o << "Group:        " << permToString(getGroupPermissions()) << endl;
	o << "World:        " << permToString(getWorldPermissions()) << endl;

	o << "Hard links:   " << getHardLinkCount() << endl;
	o << "UID:          " << getUID() << endl;
	o << "GID:          " << getGID() << endl;

	o << "Size:         " << getSize() << endl;

	time_t t = system_clock::to_time_t(getA_time());
	o << "atime:        " << ctime(&t);

	t = system_clock::to_time_t(getM_time());
	o << "mtime:        " << ctime(&t);

	t = system_clock::to_time_t(getC_time());
	o << "ctime:        " << ctime(&t);
}
