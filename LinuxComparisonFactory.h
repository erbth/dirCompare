#ifndef _LINUX_COMPARISON_FACTORY_H
#define _LINUX_COMPARISON_FACTORY_H

#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "ComparisonStrategyFactory.h"

using namespace std;

class LinuxComparisonFactory : public ComparisonStrategyFactory
{
public:
	LinuxComparisonFactory(shared_ptr<SystemParameters> sp);

	virtual vector<shared_ptr<FileComparisonStrategy>>
		createFileStrategies() override;

	virtual vector<shared_ptr<DirectoryComparisonStrategy>>
		createDirStrategies() override;
};

#endif /* _LINUX_COMPARISON_FACTORY_H */
