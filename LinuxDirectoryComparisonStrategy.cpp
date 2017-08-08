#include <memory>
#include "SystemParameters.h"
#include "LinuxDirectoryComparisonStrategy.h"

LinuxDirectoryComparisonStrategy::LinuxDirectoryComparisonStrategy(
	shared_ptr<SystemParameters> sp)
	: DirectoryComparisonStrategy(sp)
{
}
