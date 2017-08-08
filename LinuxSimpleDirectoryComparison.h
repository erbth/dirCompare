#ifndef _LINUX_SIMPLE_DIRECTORY_COMPARISON_H
#define _LINUX_SIMPLE_DIRECTORY_COMPARISON_H

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "Directory.h"
#include "LinuxDirectoryComparisonStrategy.h"

class LinuxSimpleDirectoryComparison : public LinuxDirectoryComparisonStrategy
{
public:
	LinuxSimpleDirectoryComparison(shared_ptr<SystemParameters> sp);

	virtual bool compare(
		shared_ptr<const Directory> d1,
		shared_ptr<const Directory> d2) const override;

	virtual const string getID() const override;
	virtual const string getDescription() const override;
};

#endif /* _LINUX_SIMPLE_DIRECTORY_COMPARISON_H */
