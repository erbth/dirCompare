#ifndef _LINUX_COMPARE_VISITOR_H
#define _LINUX_COMPARE_VISITOR_H

#include <string>
#include <vector>
#include "File.h"
#include "Directory.h"
#include "DirectoryCompareStrategy.h"
#include "FileCompareStrategy.h"
#include "CompareVisitor.h"

class LinuxCompareVisitor : public CompareVisitor
{
public:
	virtual LinuxCompareVisitor;

	virtual const string *compareDirectories(Directory *master, Directory *slave) override;
	virtual const string *compareFiles(File *master, File *slave) override;
}

#endif /* _LINUX_COMPARE_VISITOR_H */
