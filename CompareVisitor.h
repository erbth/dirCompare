#ifndef _COMPARE_VISITOR_H
#define _COMPARE_VISITOR_H

#include "File.h"
#include "Directory.h"
#include "DirectoryCompareStrategy.h"
#include "FileCompareStrategy.h"
#include <string>
#include <vector>

class CompareVisitor
{
protected:
	vector<DirectoryCompareStrategy*> directoryStrategies;
	vector<FileCompareStrategy*> fileStrategies;

	DirectoryCompareStrategy *directoryStrategy = nullptr;
	FileCompareStrategy *fileStrategy = nullptr;

public:
	virtual CompareVisitor() = 0;
	virtual ~CompareVisitor();

	virtual const string *compareDirectories(Directory *master, Directory *slave) = 0;
	virtual const string *compareFiles(File *master, File *slave) = 0;

	vector<DirectoryCompareStrategy> *getDirectoryCompareStrategies();
	vector<FileCompareStrategy> *getFileCompareStrategies();

	vector<DirectoryCompareStrategy> *getDirectoryCompareStrategy();
	vector<FileCompareStrategy> *getFileCompareStrategy();

	void SetDirectoryCompareStrategy(DirectoryCompareStrategy *strategy);
	void SetFileCompareStrategy(FileCompareStrategy *strategy);
}

#endif /* _COMPARE_VISITOR_H */
