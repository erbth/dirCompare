#ifndef _LINUX_DIRECTORY_COMPARE_STRATEGY
#define _LINUX_DIRECTORY_COMPARE_STRATEGY

#include <string>
#include "Directory.h"
#include "DirectoryCompareStrategy.h"

class LinuxDirectoryCompareStrategy : public DirectoryCompareStrategy
{
public:
	virtual const string *compare(Directory *master, Directory *slave) override;
}

#endif /* _LINUX_DIRECTORY_COMPARE_STRATEGY */
