#ifndef _LINUX_DIRECTORY_COMPARISON_STRATEGY_H
#define _LINUX_DIRECTORY_COMPARISON_STRATEGY_H

#include <memory>
#include "SystemParameters.h"
#include "DirectoryComparisonStrategy.h"

using namespace std;

class LinuxDirectoryComparisonStrategy : public DirectoryComparisonStrategy
{
public:
	LinuxDirectoryComparisonStrategy(shared_ptr<SystemParameters> sp);
};

#endif /* _LINUX_DIRECTORY_COMPARISON_STRATEGY_H */
