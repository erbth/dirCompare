#include <vector>
#include <string>
#include "LinuxCompareVisitor.h"
#include "CompareStrategy.h"
#include "LinuxSimpleFileCompareStrategy.h"
#include "LinuxSimpleDirectoryCompareStrategy.h"

LinuxCompareVisitor::LinuxCompareVisitor()
{
	fileStrategies.push_back(new LinuxSimpleFileCompareStrategy());
	directoryStrategies.push_back(new LinuxSimpleDirectoryCompareStrategy());
}

const string *LinuxCompareVisitor::compareDirectories(Directory *master, Directory *slave)
{
	if (directoryStrategy == nullptr)
	{
		throw new string("directory compare strategy not set\n");
	}

	return directoryStrategy->compare(master, slave);
}

const string *LinuxCompareVisitor::compareFiles
