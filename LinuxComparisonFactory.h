#ifndef _LINUX_COMPARISON_FACTORY_H
#define _LINUX_COMPARISON_FACTORY_H

#include <vector>
#include <memory>
#include "FileComparisonStrategy.h"
#include "ComparisonStrategyFactory.h"

using namespace std;

class LinuxComparisonFactory : public ComparisonStrategyFactory
{
public:
	virtual vector<shared_ptr<FileComparisonStrategy>>
		createFileStrategies() override;
};

#endif /* _LINUX_COMPARISON_FACTORY_H */
