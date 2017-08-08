#ifndef _LINUX_FILE_COMPARISON_STRATEGY_H
#define _LINUX_FILE_COMPARISON_STRATEGY_H

#include <memory>
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"

class LinuxFileComparisonStrategy : public FileComparisonStrategy
{
public:
	LinuxFileComparisonStrategy(shared_ptr<SystemParameters> sp);
};

#endif /* _LINUX_FILE_COMPARISON_STRATEGY_H */
