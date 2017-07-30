#ifndef _DIRECTORY_COMPARE_STRATEGY
#define _DIRECTORY_COMPARE_STRATEGY

#include <string>
#include "Directory.h"

class DirectoryCompareStrategy
{
public:
	virtual const string *compare(Directory *master, Directory *slave) = 0;
}

#endif /* _DIRECTORY_COMPARE_STRATEGY */
