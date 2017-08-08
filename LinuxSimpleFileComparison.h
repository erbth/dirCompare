#ifndef _LINUX_SIMPLE_FILE_COMPARISON_H
#define _LINUX_SIMPLE_FILE_COMPARISON_H

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "File.h"
#include "LinuxFileComparisonStrategy.h"

using namespace std;

class LinuxSimpleFileComparison : public LinuxFileComparisonStrategy
{
public:
	LinuxSimpleFileComparison(shared_ptr<SystemParameters> sp);

	virtual bool compare(
		shared_ptr<const File> f1,
		shared_ptr<const File> f2) const override;

	virtual const string getID() const override;
	virtual const string getDescription() const override;
};

#endif /* _LINUX_SIMPLE_FILE_COMPARISON_H */
