#ifndef _LINUX_FILE_COMPARE_STRATEGY
#define _LINUX_FILE_COMPARE_STRATEGY

#include <string>
#include "File.h"
#include "FileCompareStrategy.h"

class LinuxFileCompareStrategy : public FileCompareStrategy
{
public:
	virtual const string *compare(File *master, File *slave) override;
}

#endif /* _LINUX_FILE_COMPARE_STRATEGY */
