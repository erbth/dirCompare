#ifndef _FILE_COMPARE_STRATEGY
#define _FILE_COMPARE_STRATEGY

#include <string>
#include "File.h"

class FileCompareStrategy
{
public:
	virtual const string *compare(File *master, File *slave) = 0;
}

#endif /* _FILE_COMPARE_STRATEGY */
