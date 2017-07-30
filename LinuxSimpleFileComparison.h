#ifndef _LINUX_SIMPLE_FILE_COMPARISON_H
#define _LINUX_SIMPLE_FILE_COMPARISON_H

#include <string>
#include "File.h"
#include "LinuxFileComparisonStrategy.h"

using namespace std;

class LinuxSimpleFileComparison : public LinuxFileComparisonStrategy
{
public:
	virtual bool compare(const File *f1, const File *f2) override;
	virtual const string getID() override;
};

#endif /* _LINUX_SIMPLE_FILE_COMPARISON_H */
